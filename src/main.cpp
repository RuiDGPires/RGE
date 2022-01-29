#include "gbc/gameboy.hpp"
#include "common/assert.hpp"
#include <iostream>

int main(int argc, char *argv[]){
    ASSERT(argc == 2, "Please specify a rom to load\n");

    GameBoy gb; 

    gb.load_rom(argv[1]);

    gb.print_cartridge_info();

    gb.turn_on();
    return 0;
}
