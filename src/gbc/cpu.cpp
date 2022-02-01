#include "cpu.hpp"

const char *instruction_lookup[] = {
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "IN_RLC", 
    "IN_RRC",
    "IN_RL", 
    "IN_RR",
    "IN_SLA", 
    "IN_SRA",
    "IN_SWAP", 
    "IN_SRL",
    "IN_BIT", 
    "IN_RES", 
    "IN_SET"
};

const SharpSM83::instruction SharpSM83::get_instruction(u8 opcode){
    return lookup[opcode];
}

const char *SharpSM83::instruction_name(instruction_type t){
    return instruction_lookup[t];
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
        // Fetch instruction
        fetch_info.op_code = this->read(regs[PC]++);

        fetch_info.inst = get_instruction(fetch_info.op_code);
        fetch_info.is_dest_addr = false;
        fetch_info.data = 0;
        fetch_info.dest = 0;

        switch (fetch_info.inst.mode){
            case AM_IMP:
                break;
            case AM_R_D16:
                fetch_info.data |= (this->read(regs[PC]++) << 8) & 0xFF00;
                fetch_info.data |= (this->read(regs[PC]++)) & 0xFF;

                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_R_R:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);

                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_MR_R:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
                fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
                fetch_info.is_dest_addr = true;
                break;
            case AM_R:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_1);
                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_R_D8:
                fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_R_MR:
                fetch_info.data = this->bus->read(this->read_reg(fetch_info.inst.reg_2));
                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_R_MHLI:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
                this->write_reg(RT_HL, fetch_info.data + 1);
                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_R_MHLD:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
                this->write_reg(RT_HL, fetch_info.data - 1);
                fetch_info.dest = fetch_info.inst.reg_1;
                break;
            case AM_MHLI_R:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
                fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
                this->write_reg(RT_HL, fetch_info.dest + 1);
                fetch_info.is_dest_addr = true;
                break;
            case AM_MHLD_R:
                fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
                fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
                this->write_reg(RT_HL, fetch_info.dest - 1);
                fetch_info.is_dest_addr = true;
                break;
            case AM_R_A8:
                break;
            case AM_A8_R:
                break;
            case AM_MHL_SPR:
                break;
            case AM_D16:
                break;
            case AM_D8:
                break;
            case AM_D16_R:
                break;
            case AM_MR_D8:
                break;
            case AM_MR:
                break;
            case AM_A16_R:
                break;
            case AM_R_A16:
                break;
        }
    }

    cycles--;
}

void SharpSM83::reset(){
    for (int i = 0; i < 6; i++)
        this->regs[i] = 0;
    this->regs[PC] = 0x100;
    this->running = false;
}

