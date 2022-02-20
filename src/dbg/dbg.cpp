#ifndef DEBUG
#define DEBUG
#endif

#include "../gbc/mem_map.hpp"
#include "../gbc/cpu.hpp"
#include "../gbc/gameboy.hpp"
#include "../common/assert.hpp"
#include "confparser.hpp"
#include "input.hpp"

#include <termios.h>
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

#define BLUE "\e[1;36m"
#define YELLOW "\e[1;33m"
#define BOLD "\e[1;37m"
#define GREEN "\e[1;32m"
#define reset "\e[0m"

static struct termios saved_attributes;

static std::vector<std::string> rom_str;
static std::vector<u32> str_map;

static std::string test_msg = "";
static ConfParser conf;

static bool changed = false;

static GameBoy gb;

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

void reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
  struct termios tattr;

/* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

/* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

/* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON | ECHO);	/* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
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
#define MAX_ROWS 10
#define EXTRA_LINES 3
#define ROW_SIZE 16

static u32 mem_page = WRA0_BEGIN / (MAX_ROWS * ROW_SIZE);

static void print_info(){
    if (conf.clear_term)
        system("clear");

    printf(" CART\n");
    printf("------\n");
    gb.slot->print_info();
    

	printf("\n REGS \n");
	printf("--------\n");
	for (u32 i = 0; i < 6; i++){
		printf("[%s]: %s%s\n%s", (reg_str[i*2+1] + reg_str[i*2+2]).c_str(), gb.cpu.regs[i] != regs[i] ? GREEN : "", hex(gb.cpu.regs[i]).c_str(), reset);
		regs[i] = gb.cpu.regs[i];
	}	

	printf("\n CODE \n");
	printf("--------\n");

    for (int i = -EXTRA_LINES; i <= EXTRA_LINES; i++){
        u32 pc = gb.cpu.regs[PC];
        u32 line = str_map[pc];

        if (i == 0){
            std::cout << YELLOW << rom_str[line] << reset;
            std::cout << "\t\t[" << hex(pc) << "]\n";
        }else if (line + i >= 0 && line + i < rom_str.size())
            std::cout << rom_str[line + i] << "\n";
        else
            std::cout << "\n";
    }

	printf("\n MEMORY \n");
	printf("--------\n");

    for (u32 j = 0; j < MAX_ROWS; j++){ // for each line
        u32 first = mem_page * MAX_ROWS * ROW_SIZE;
        
        char line_ascii[ROW_SIZE + 1];
       line_ascii[ROW_SIZE] = '\0';

        std::cout << get_mem_name(first + j*ROW_SIZE) << " - " << hex(first + j*ROW_SIZE) << " | ";
        for (u32 i = 0; i < ROW_SIZE; i++){
            if (first + i + j*ROW_SIZE > IE_END)
                goto end;

            u8 val = gb.mem_bus.read(first + i + j*ROW_SIZE);
            std::cout << hex(val, false, 2) << " ";
            line_ascii[i] = ascii_rep(val);
        }

        std::cout << "\t" << line_ascii;

        std::cout << "\n";
    }
end:

    printf("\n RET \n");
	printf("--------\n");
    
    std::cout << test_msg << std::endl;
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

#define CTRL_KEY(k) ((k)  & 0x1f)

static bool ctrl(char c){
    if (c == 53)
        return true;
    return false;
}

/* I this doens't work and don't know why */
static __attribute__((unused)) bool shift(char c){
    if (c == 50)
        return true;
    return false;
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

bool show_info = conf.info;
bool prompt = true;
bool step = true;
bool to_exit = false;

//**********************************************************************************************
//------------------
// KEY PRESS EVENTS
//------------------

EVENT(K_ARROW_UP){
    page_dec(1);
    step = false;
}
EVENT(K_ARROW_DOWN){
    page_inc(1);
    step = false;
}
EVENT(K_CTRL_ARROW_UP){
    mem_page = 0;
    step = false;
}
EVENT(K_CTRL_ARROW_DOWN){
    mem_page = (IE_END / (MAX_ROWS * ROW_SIZE));
    step = false;
}

EVENT(K_ENTER){
    bool res = gb_step();
    prompt = res;
    show_info = res ? true : conf.info;
    step = false;
}

EVENT(K_CTRL_X){
    to_exit = true;
}

EVENT(K_CTRL_R){
    fetch_mem();
    step = false;
}

//**********************************************************************************************


static void run(){
	// Pass the control to the VM
	set_input_mode();
    gb.cpu.running = true;
	do {
        if (show_info){
            // Display information
            print_info();
            step = true;

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

    reset_input_mode();
}


#ifndef MAIN
int main(int argc, char *argv[]){
	ASSERT(argc >= 2, "Invalid number of arguments");
    setup_events();

    ENABLE_KEY(K_ARROW_UP);
    ENABLE_KEY(K_ARROW_DOWN);
    ENABLE_KEY(K_CTRL_ARROW_UP);
    ENABLE_KEY(K_CTRL_ARROW_DOWN);
    ENABLE_KEY(K_ENTER);
    ENABLE_KEY(K_CTRL_X);
    ENABLE_KEY(K_CTRL_R);

    if (argc == 3)
        conf.parse(argv[2]);
	
    gb.load_rom(argv[1]);
    fetch_mem();

    run();

    return 0;
}
#endif
