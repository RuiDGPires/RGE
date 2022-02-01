#include "gbc/gameboy.hpp"
#include "common/assert.hpp"
#include <iostream>

int main(int argc, char *argv[]){
    ASSERT(argc == 2, "Please specify a rom to load\n");

    GameBoy gb; 

    gb.load_rom(argv[1]);

    gb.print_cartridge_info();

    gb.turn_on();

    gb.turn_off();
    return 0;
}

/*
void print_regs(SharpSM83 &cpu){
    printf("AF: %x\n", cpu.read_reg(cpu.RT_AF));
    printf("BC: %x\n", cpu.read_reg(cpu.RT_BC));
    printf("DE: %x\n", cpu.read_reg(cpu.RT_DE));
    printf("HL: %x\n", cpu.read_reg(cpu.RT_HL));
    printf("B: %x\n", cpu.read_reg(cpu.RT_B));
    printf("C: %x\n", cpu.read_reg(cpu.RT_C));
    printf("\n");
}


int main(int argc, char *argv[]){
    SharpSM83 cpu;

    cpu.write_reg(cpu.RT_A, 0x10);
    print_regs(cpu);
    cpu.write_reg(cpu.RT_F, 0x02);
    cpu.write_reg(cpu.RT_A, 0x44);
    print_regs(cpu);
    cpu.write_reg(cpu.RT_BC, 0x1030);
    print_regs(cpu);
    cpu.write_reg(cpu.RT_C, 0x55);

    cpu.write_reg(cpu.RT_H, 0x27);
    cpu.write_reg(cpu.RT_L, 0x41);
    print_regs(cpu);
    cpu.write_reg(cpu.RT_HL, 0x5524);
    print_regs(cpu);
    cpu.write_reg(cpu.RT_H, 0x83);
    cpu.write_reg(cpu.RT_L, 0x83);
    cpu.write_reg(cpu.RT_H, 0x22);

    print_regs(cpu);
}
*/
