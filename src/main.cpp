#ifndef DEBUG
#define DEBUG
#endif

#include "gbc/mem_map.hpp"
#include "gbc/cpu.hpp"
#include "gbc/gameboy.hpp"
#include "common/assert.hpp"
#include "dbg/screen.hpp"
#include "dbg/confparser.hpp"
#include "dbg/input.hpp"

#include <unistd.h>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

static const std::string reg_str[13] = {
    "<NONE>",
    "A",
    "F",
    "B",
    "C",
    "D",
    "E",
    "H",
    "L",
    "S",
    "P",
    "P",
    "C"
};

static std::string writing_mode();
static void execute_command(std::string);

static std::vector<std::string> rom_str;
static std::vector<u32> str_map;

static std::string test_msg = "";
static ConfParser conf;

static bool changed = false;

static GameBoy gb;

#define MAX_ROWS 16 
#define EXTRA_LINES 7 
#define ROW_SIZE 16

//---------------------------
// Screen and it's components
//---------------------------

Screen screen;

TitledTextBox txt_regs("Registers", 0, 0, 27, MAX_ROWS+4), 
              txt_code("Code", 0, MAX_ROWS + 5, 27, MAX_ROWS+4), 
              txt_mem("Memory", 28, MAX_ROWS + 5, 87, MAX_ROWS+4),
              txt_cart("Cartridge", 28, 0, 87, MAX_ROWS+4);
TextBox txt_result(0, 41, 50, 9);
ScrollingTextBox console(27+87+2, 0, 40, MAX_ROWS*2+9);

Footer footer(0, screen.term_w);

bool check_test(){
    if (test_msg[test_msg.size() - 1])
        test_msg.erase(test_msg.end() - 1, test_msg.end());
    std::stringstream stream(test_msg);

    std::string line;
    while(getline(stream, line)); // get last line

    return line == "Passed";
}

void check_test_char(){
    if (gb.mem_bus.read(0xFF02) == 0x81){
        changed = true;
        test_msg.push_back(gb.mem_bus.read(0xFF01));
        gb.mem_bus.write(0xFF02, 0);
    }
}

char ascii_rep(u8 byte){
    if (byte >= ' ' && byte <= '~')
        return (char) byte;
    else
        return '.';
}

static std::string decode_reg(SharpSM83::reg_type reg){
    if (reg == SharpSM83::reg_type::RT_NONE)
        return reg_str[0];

    u8 order = (reg >> 16);

    std::string str = "";

    if (reg & 0xFF00)
        str += reg_str[order*2 + 1];
    if (reg & 0x00FF)
        str += reg_str[order*2 + 2];

    return str;
}

static std::string hex(unsigned int val, bool prefix = true, u32 n = 4){
    char tmp[10];
    sprintf (tmp, "%x", val);
    u8 rest = strlen(tmp) % n != 0? n - (strlen (tmp) % n): 0;
    std::string str = (prefix? std::string ("0x"): "") + std::string (rest, '0');
    for (size_t i = 0; i < strlen (tmp); i++){
        if ((i + rest) % n == 0 && i != 0)
            str += " ";
        str += tmp[i];
    }
    return str;
}  

static void append(std::string &str, std::string a){
    str.push_back(' ');
    str.append(a);
}

static std::string envolve(std::string str){
    return "(" + str + ")";
}

static void fetch_mem(){
    rom_str = std::vector(IE_END, std::string("<NONE>"));
    str_map = std::vector(IE_END, (u32) 0);
    SharpSM83::instruction inst;
    SharpSM83 dummy;

    for (u32 i = 0x0, count = 0; i < IE_END; count++){
        std::string str = "";

        inst = dummy.lookup[gb.mem_bus.read(i)];

        str += inst.str;

        str_map[i++] = count;

        using a = SharpSM83;
        if (inst.mode == &a::AM_IMP){
            if (inst.type == &a::IT_STOP){
                i++;
            }
        }else if (inst.mode == &a::AM_R_D16){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            u32 val = gb.mem_bus.read(i++) & 0x00FF;
            val |= (((u32) gb.mem_bus.read(i++)) << 8) & 0xFF00;
            append(str, hex(val));

        }else if (inst.mode == &a::AM_R_R){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MR_R){
            append(str, envolve(decode_reg(inst.reg_1)));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_R){
            append(str, decode_reg(inst.reg_1));

        }else if (inst.mode == &a::AM_R_D8){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, hex(gb.mem_bus.read(i++)));

        }else if (inst.mode == &a::AM_R_MR){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            std::string aux = "";
            if (inst.reg_1 == SharpSM83::RT_A && inst.reg_2 == SharpSM83::RT_C)
                aux = "0xFF00 + ";

            append(str, envolve(aux + decode_reg(inst.reg_2)));

        }else if (inst.mode == &a::AM_R_MHLI){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, envolve(decode_reg(inst.reg_2) + "+"));

        }else if (inst.mode == &a::AM_R_MHLD){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, envolve(decode_reg(inst.reg_2) + "-"));

        }else if (inst.mode == &a::AM_MHLI_R){
            append(str, envolve(decode_reg(inst.reg_1) + "+"));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MHLD_R){
            append(str, envolve(decode_reg(inst.reg_1) + "-"));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_R_A8){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, envolve("0xFF00 + " + hex(gb.mem_bus.read(i++))));

        }else if (inst.mode == &a::AM_A8_R){
            append(str, envolve("0xFF00 + " + hex(gb.mem_bus.read(i++))));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MHL_SPR){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, "$SP + " + hex(gb.mem_bus.read(i++)));

        }else if (inst.mode == &a::AM_D16){
            u32 val = gb.mem_bus.read(i++) & 0x00FF;
            val |= (((u32) gb.mem_bus.read(i++)) << 8) & 0xFF00;
            append(str, hex(val));

        }else if (inst.mode == &a::AM_D8){
            append(str, hex(gb.mem_bus.read(i++)));

        }else if (inst.mode == &a::AM_D16_R){
            u32 val = gb.mem_bus.read(i++) & 0x00FF;
            val |= (((u32) gb.mem_bus.read(i++)) << 8) & 0xFF00;
            append(str, hex(val));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MR_D8){
            append(str, envolve(decode_reg(inst.reg_1)));
            str.push_back(',');
            append(str, hex(gb.mem_bus.read(i++)));

        }else if (inst.mode == &a::AM_MR){
            append(str, envolve(decode_reg(inst.reg_1)));

        }else if (inst.mode == &a::AM_A16_R){
            u32 val = gb.mem_bus.read(i++) & 0x00FF;
            val |= (((u32) gb.mem_bus.read(i++)) << 8) & 0xFF00;

            std::string aux = "";
            if (inst.reg_2 == SharpSM83::RT_A)
                aux = "0xFF00 + ";

            append(str, envolve(aux + hex(val)));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));
        }else if (inst.mode == &a::AM_R_A16){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            u32 val = gb.mem_bus.read(i++) & 0x00FF;
            val |= (((u32) gb.mem_bus.read(i++)) << 8) & 0xFF00;

            std::string aux = "";
            if (inst.reg_1 == SharpSM83::RT_A)
                aux = "0xFF00 + ";
            append(str, envolve(aux + hex(val)));
        }

        rom_str[count] = str;
    }
}

u16 regs[6];
static u32 mem_page = WRA0_BEGIN / (MAX_ROWS * ROW_SIZE);

static void print_info(){
    
    // REGS
    txt_regs.clear();
	for (u32 i = 0; i < 6; i++){
        txt_regs << '[' << reg_str[i*2+1] + reg_str[i*2+2] << ']' << ": " << (gb.cpu.regs[i] != regs[i] ? GREEN_c : RESET_c) << hex(gb.cpu.regs[i]) << RESET_c << NL;

		regs[i] = gb.cpu.regs[i];
	}	

    // CODE
    txt_code.clear();
    for (int i = -EXTRA_LINES; i <= EXTRA_LINES; i++){
        u32 pc = gb.cpu.regs[PC];
        u32 line = str_map[pc];

        if (i == 0){
            txt_code << YELLOW_c << rom_str[line] << RESET_c;
            txt_code << "  [" << hex(pc) << "]" << NL;
        }else if (line + i >= 0 && line + i < rom_str.size())
            txt_code << rom_str[line + i] << NL;
        else
            txt_code << NL;
    }

    // MEMORY
    txt_mem.clear();
    for (u32 j = 0; j < MAX_ROWS; j++){ // for each line
        u32 first = mem_page * MAX_ROWS * ROW_SIZE;
        
        char line_ascii[ROW_SIZE + 1];
        line_ascii[ROW_SIZE] = '\0';

        if (first + j*ROW_SIZE > IE_END)
            break;

        txt_mem << get_mem_name(first + j*ROW_SIZE) << " - " << hex(first + j*ROW_SIZE) << " | ";
        for (u32 i = 0; i < ROW_SIZE; i++){
            if (first + i + j*ROW_SIZE > IE_END)
                goto end;

            u8 val = gb.mem_bus.read(first + i + j*ROW_SIZE);
            txt_mem << hex(val, false, 2) << " ";
            line_ascii[i] = ascii_rep(val);
        }

        txt_mem << "   " << line_ascii;

        txt_mem << NL;
    }
end:

    txt_result.clear() << test_msg << NL;

    screen.refresh();
}


static bool gb_step(){
    gb.cpu.clock();
    check_test_char();

    bool is_test;
    bool ret = conf.check(gb, &is_test);
    
    if (is_test)
        exit(!check_test());

    return ret;
}

static void page_inc(u32 n){
    if (n == 0) return;
    if ((mem_page + 1) * MAX_ROWS * ROW_SIZE < IE_END)
        mem_page += n;
    else
        page_inc(n-1);
}

static void page_dec(u32  n){
    mem_page -= (mem_page > n)? n: mem_page;
}

bool show_info;
bool prompt = true;
bool step = true;
bool to_exit = false;

//**********************************************************************************************
//------------------
// KEY PRESS EVENTS
//------------------

EVENT(K_SPACE){
    step = true;
}

EVENT(K_ARROW_UP){
    page_dec(1);
}

EVENT(K_ARROW_DOWN){
    page_inc(1);
}

EVENT(K_CTRL_ARROW_UP){
    mem_page = 0;
}

EVENT(K_CTRL_ARROW_DOWN){
    mem_page = (IE_END / (MAX_ROWS * ROW_SIZE));
}

EVENT(K_ENTER){
    bool res = gb_step();
    prompt = res;
    show_info = res ? true : conf.info;
}

EVENT(K_CTRL_X){
    to_exit = true;
}

EVENT(K_CTRL_R){
    fetch_mem();
}

EVENT(K_CLN){
    std::string command = writing_mode();
    if (command != "")
        execute_command(command);
}

EVENT(K_DEL){
    console << "\nDelete pressed";
}

//**********************************************************************************************

static void run(){
	// Pass the control to the VM
    gb.cpu.running = true;
	do {
        if (show_info){
            // Display information
            print_info();
            step = false;

            if (prompt)
                wait_input();

            if (to_exit)
                break;

            if (step){
                bool b = gb_step();
                if (!prompt)
                   prompt = b; 
        }
        }else{
            bool res = gb_step();
            prompt = res;
            show_info = res;
        }
	}while(gb.cpu.running);

    if (!show_info)
        print_info();

}

int main(int argc, char *argv[]){
	ASSERT(argc >= 2, "Invalid number of arguments");
    setup_events();

    ENABLE_KEY(K_SPACE);
    ENABLE_KEY(K_ARROW_UP);
    ENABLE_KEY(K_ARROW_DOWN);
    ENABLE_KEY(K_CTRL_ARROW_UP);
    ENABLE_KEY(K_CTRL_ARROW_DOWN);
    ENABLE_KEY(K_ENTER);
    ENABLE_KEY(K_CTRL_X);
    ENABLE_KEY(K_CTRL_R);
    ENABLE_KEY(K_CLN);
    ENABLE_KEY(K_DEL);

    txt_result.centered = true;
    screen << txt_regs << txt_code << txt_mem << txt_result << txt_cart << footer << console;

    if (argc == 3)
        conf.parse(argv[2]);
	
    show_info = conf.info;
    
    gb.load_rom(argv[1]);

    txt_cart.clear() << gb.slot->info();

    footer << "Input file: " << argv[1];
    fetch_mem();

    screen.refresh();
    run();

    return 0;
}

//---------------------------------------------------------
//***************
// WRITING MODE
//***************

static std::string put_cursor(std::string &s, u32 cursor){
    const std::string invert_colors = "\033[7m";

    u32 size = s.size();
    if (cursor >= size)
        return s + invert_colors + " " + RESET_STR;
    else if (cursor == size -1)
        return s.substr(0, cursor) + invert_colors + s[cursor] + RESET_STR;
    else
        return s.substr(0, cursor) + invert_colors + s[cursor] + RESET_STR + s.substr(cursor + 1, size - cursor - 1);

    return "";
}

static std::string writing_mode(){
    std::vector<std::string> footer_vec = footer.str();

    std::string text = "", footer_back = footer_vec.empty() ? "": footer_vec[0];

    u32 cursor = 0;

    footer.clear() << ":" + put_cursor(text, cursor);
    screen.refresh();

    while(1){
        Key key = get_key();
        
        char c = key_to_ascii(key);
        if (c){
            text.insert(text.begin()+cursor, c);
            cursor++;
        }
        else switch (key){
            case K_ESC:
                text = "";
            case K_ENTER:
                goto _wm_end;
            case K_ARROW_LEFT:
                if (cursor > 0)
                    cursor--;
                break;
            case K_ARROW_RIGHT:
                if (cursor < text.size())
                    cursor++;
                break;
            case K_BCKSPACE:
                if (cursor > 0){
                    text.erase(text.begin()+cursor-1);
                    cursor--;
                }
                break;
            case K_DEL:
                if (cursor < text.size()){
                    text.erase(text.begin()+cursor);
                }
                break;
            default:
                break;
        }
        footer.clear() << ":" + put_cursor(text, cursor);
        screen.refresh();
    }

_wm_end:
    footer.clear() << footer_back;
    screen.refresh();
    return text; 
}

//---------------------------------------------------------
//***************
// COMMANDS
//***************

static void execute_command(std::string command){
    if (command == "q")
        to_exit = true;
}
