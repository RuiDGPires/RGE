#include "cpu.hpp"

const SharpSM83::instruction SharpSM83::get_instruction(u8 opcode){
    return lookup[opcode];
}

const char *SharpSM83::instruction_name(instruction inst){
    return inst.str;
}

SharpSM83::SharpSM83(){
    this->reset();
}

SharpSM83::~SharpSM83(){

}

void SharpSM83::connect_bus(Bus *bus){
    this->bus = bus;
}

void SharpSM83::write(u16 addr, u8 data){
    this->bus->write(addr, data);
}

u8 SharpSM83::read(u16 addr){
    return this->bus->read(addr);
}

void SharpSM83::write_reg(reg_type reg_t, u16 data){
    u8 reg = reg_t >> 16;
    u16 mask = reg_t & 0xFFFF;

    this->regs[reg] &= ~(mask);

    if (mask & 0xFF){
        this->regs[reg] |= (data & 0xFF);
    }else{
        data <<= 8;
    }

    this->regs[reg] |= data & 0xFF00;
}

u16 SharpSM83::read_reg(reg_type reg_t){
    u8 reg = reg_t >> 16;

    u16 mask = reg_t & 0xFFFF;
    u16 data = this->regs[reg]; 

    if (!(mask & 0xFF))
        data >>= 8;
    
    return data & (mask | 0xFF);
}

void SharpSM83::clock(){
    if (cycles == 0){
        fetch_info = {0, {0, "---", &a::IT_NONE}, false, 0, 0};
        // Fetch instruction
        fetch_info.op_code = this->read(regs[PC]++);
        fetch_info.inst = get_instruction(fetch_info.op_code);

        cycles = fetch_info.inst.cycles;

        (this->*fetch_info.inst.mode)();
        if ((this->*fetch_info.inst.type)())
                cycles += fetch_info.inst.extra_cycles;
    }

    cycles--;
}

void SharpSM83::reset(){
    for (int i = 0; i < 6; i++)
        this->regs[i] = 0;
    this->regs[PC] = 0x0100;
    this->regs[SP] = 0xFFFE;
    this->running = false;
}

