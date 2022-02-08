#ifndef DEBUG
#define DEBUG
#endif


#define CLEAR_TERM 1

#include "../gbc/cpu.hpp"
#include "../gbc/gameboy.hpp"
#include "../common/assert.hpp"

#include <termios.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>


#define BLUE "\e[1;36m"
#define YELLOW "\e[1;33m"
#define BOLD "\e[1;37m"
#define GREEN "\e[1;32m"
#define reset "\e[0m"

struct termios saved_attributes;

std::vector<std::string> rom_str;
std::vector<u32> str_map;

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


static std::string hex(unsigned int val, bool prefix = true, u32 n = 4){
    char tmp[10];
    sprintf (tmp, "%x", val);
    u8 rest = strlen(tmp) % n != 0? n - (strlen (tmp) % n): 0;
    std::string str = prefix? std::string ("0x"): "" + std::string (rest, '0');
    for (size_t i = 0; i < strlen (tmp); i++){
        if ((i + rest) % n == 0 && i != 0)
            str += " ";
        str += tmp[i];
    }
    return str;
}  

const std::string reg_str[13] = {
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

static void append(std::string &str, std::string a){
    str.push_back(' ');
    str.append(a);
}

static std::string envolve(std::string str){
    return "(" + str + ")";
}



static void fetch_rom(Cartridge &cart){
    rom_str = std::vector(cart.size, std::string("<NONE>"));
    str_map = std::vector(cart.size, (u32) 0);
    SharpSM83::instruction inst;
    SharpSM83 dummy;


    std::cout << "Cart size: " << cart.size << "\n";
    fflush(stdout);
    for (u32 i = 0x100, count = 0; i < cart.size; count++){
        std::string str = "";

        inst = dummy.lookup[cart.data[i]];

        str += inst.str;

        str_map[i++] = count;

        using a = SharpSM83;
        if (inst.mode == &a::AM_IMP)
            ;
        else if (inst.mode == &a::AM_R_D16){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            u32 val = (((u32) cart.data[i++]) << 8) & 0xFF00;
            val |= cart.data[i++] & 0x00FF;
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
            append(str, hex(cart.data[i++]));

        }else if (inst.mode == &a::AM_R_MR){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            std::string aux = "";
            if (inst.reg_1 == SharpSM83::RT_A && inst.reg_2 == SharpSM83::RT_C)
                aux = "0xFF00 + ";

            append(str, envolve(aux + decode_reg(inst.reg_1)));


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
            append(str, envolve("0xFF00 + " + hex(cart.data[i++])));

        }else if (inst.mode == &a::AM_A8_R){
            append(str, envolve("0xFF00 + " + hex(cart.data[i++])));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MHL_SPR){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            append(str, "$SP + " + hex(cart.data[i++]));

        }else if (inst.mode == &a::AM_D16){
            u32 val = (((u32) cart.data[i++]) << 8) & 0xFF00;
            val |= cart.data[i++] & 0x00FF;
            append(str, hex(val));

        }else if (inst.mode == &a::AM_D8){
            append(str, hex(cart.data[i++]));

        }else if (inst.mode == &a::AM_D16_R){
            u32 val = (((u32) cart.data[i++]) << 8) & 0xFF00;
            val |= cart.data[i++] & 0x00FF;
            append(str, hex(val));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));

        }else if (inst.mode == &a::AM_MR_D8){
            append(str, envolve(decode_reg(inst.reg_1)));
            str.push_back(',');
            append(str, hex(cart.data[i++]));

        }else if (inst.mode == &a::AM_MR){
            append(str, envolve(decode_reg(inst.reg_1)));

        }else if (inst.mode == &a::AM_A16_R){
            u32 val = (((u32) cart.data[i++]) << 8) & 0xFF00;
            val |= cart.data[i++] & 0x00FF;

            std::string aux = "";
            if (inst.reg_2 == SharpSM83::RT_A)
                aux = "0xFF00 + ";

            append(str, envolve(aux + hex(val)));
            str.push_back(',');
            append(str, decode_reg(inst.reg_2));
        }else if (inst.mode == &a::AM_R_A16){
            append(str, decode_reg(inst.reg_1));
            str.push_back(',');
            u32 val = (((u32) cart.data[i++]) << 8) & 0xFF00;
            val |= cart.data[i++] & 0x00FF;

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

static u32 mem_page = RAM_BEGIN / (MAX_ROWS * ROW_SIZE);

static void print_info(GameBoy &gb){
    if (CLEAR_TERM)
        system("clear");

    printf("\n");
    printf("DEBUG MODE [%s]\n", gb.cpu.debug_mode ? "ON": "OFF");
    

	printf(" REGS \n");
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
        }else if (line + i >= 0 && line + i < gb.slot->size)
            std::cout << rom_str[line + i] << "\n";
        else
            std::cout << "\n";
    }

	printf("\n MEMORY \n");
	printf("--------\n");

    for (u32 j = 0; j < MAX_ROWS; j++){
        u32 first = mem_page * MAX_ROWS * ROW_SIZE;

        std::cout << hex(first + j*ROW_SIZE) << " | ";
        for (u32 i = 0; i < ROW_SIZE; i++){
            if (first + i + j*ROW_SIZE > RAM_END)
                goto end;

            std::cout << hex(gb.mem_bus.read(first + i+j*ROW_SIZE), false, 2) << " ";
        }

        std::cout << "\n";
    }

end:

	fflush(stdout);
}

#define ARROW_UP 'A'
#define ARROW_DOWN 'B'

static void run(GameBoy &gb){
	// Pass the control to the VM
	set_input_mode();
    gb.cpu.running = true;
	do {
        // Display information
		print_info(gb);

		char c = getchar();
        bool step = true;

        if (c == '\033'){ // key pressed{
            (void) getchar();
            switch (getchar()){
                case ARROW_UP:
                    if ((int)(mem_page - 1) >= 0)
                        mem_page--;
                    
                    step = false;
                    break;
                case ARROW_DOWN:
                    if (mem_page + 1 * MAX_ROWS * ROW_SIZE < RAM_SIZE)
                        mem_page++;

                    step = false;
                    break;
                default:
                    printf("OTHER\n");
                    break;
            }
        }
        if (step)
            gb.cpu.clock();
	}while(gb.cpu.running);
	reset_input_mode();
}

int main(int argc, char *argv[]){
	ASSERT(argc == 2, "Invalid number of arguments");
	
    GameBoy gb;

    gb.load_rom(argv[1]);
    fetch_rom(*(gb.slot));

    run(gb);

    for (u32 i = 0; i < 20; i++)
        std::cout << rom_str[i] << "\n";

    return 0;
}
