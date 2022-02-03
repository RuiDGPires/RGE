#include "cpu.hpp"
#include "../common/assert.hpp"

// TODO : flags

bool SharpSM83::IT_NONE(){
    // Unkown behaviour
    return false;
}

bool SharpSM83::IT_NOP(){
    return false;
}

bool SharpSM83::IT_LD(){
    if (fetch_info.is_dest_addr)
        this->write(fetch_info.dest, fetch_info.data);  
    else
        this->write_reg((SharpSM83::reg_type) fetch_info.dest, fetch_info.data);

    return false;
}

bool SharpSM83::IT_INC(){
    u16 val = fetch_info.data + 1;
    if (fetch_info.is_dest_addr)
        this->write(fetch_info.dest, val);
    else
        this->write_reg((SharpSM83::reg_type) fetch_info.dest, val);

    return false;
}

bool SharpSM83::IT_DEC(){
    u16 val = fetch_info.data - 1;
    if (fetch_info.is_dest_addr)
        this->write(fetch_info.dest, val);
    else
        this->write_reg((SharpSM83::reg_type) fetch_info.dest, val);

    return false;
}

bool SharpSM83::IT_RLCA(){
    fetch_info.dest = this->regs[RT_A];
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RLC();
}

bool SharpSM83::IT_ADD(){
    ASSERT(!(fetch_info.is_dest_addr), "Dest must be a register");

    // missing size specification and flags
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) + fetch_info.data;

    this->write_reg((SharpSM83::reg_type) fetch_info.dest, val & 0xFFFF);

    return false;
}

bool SharpSM83::IT_RRCA(){
    fetch_info.dest = this->regs[RT_A];
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RRC();
}

bool SharpSM83::IT_STOP(){
    return false;
}

bool SharpSM83::IT_RLA(){
    fetch_info.dest = this->regs[RT_A];
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RL();
}

bool SharpSM83::eval_condition(){
    u8 flg = read_reg(RT_F) & 0xFF;
    switch(fetch_info.inst.cond){
        case CT_C:
            return flg & flags::c;
        case CT_NC:
            return !(flg & flags::c);
        case CT_Z:
            return flg & flags::z;
        case CT_NZ:
            return !(flg & flags::z);
        default:
            return false;
    }
}

void SharpSM83::push(u8 data){
    this->write(--this->regs[SP], data);
}

void SharpSM83::push(u16 data){
    this->push((u8) ((data >> 8) & 0xFF));
    this->push((u8) (data & 0xFF));
}

u8 SharpSM83::pop(){
    return this->bus->read(this->regs[SP]++);
}

bool SharpSM83::jump(bool save_pc = false){
    if (eval_condition()){ 
        if (save_pc)
            this->push((u16) this->regs[PC]);

        this->regs[PC] = fetch_info.data;
        return true;
    }else{
        return false;
    }
}

bool SharpSM83::IT_JR(){
    fetch_info.data += this->regs[PC];
    return this->jump();
}

bool SharpSM83::IT_RRA(){
    fetch_info.dest = this->regs[RT_A];
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RR();
}

bool SharpSM83::IT_DAA(){
    // TODO
    return false;
}

bool SharpSM83::IT_CPL(){
    this->write_reg(RT_A, ~this->read(RT_A));
    return false;
}

bool SharpSM83::IT_SCF(){
    // Set carry flag
    this->write(RT_F, this->read(RT_F) | flags::c & ~flags::n & ~flags::h);
    return false;
}

bool SharpSM83::IT_CCF(){
    this->write(RT_F, (this->read(RT_F) ^ flags::c) & ~flags::n & ~flags::h);
    return false;
}

bool SharpSM83::IT_HALT(){
    this->halted = true;
    return false;
}

bool SharpSM83::IT_ADC(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) + fetch_info.data;
    u8 carry = (this->read_reg(RT_F) & flags::c) != 0;

    this->write_reg((SharpSM83::reg_type) fetch_info.dest, (val + carry) & 0xFFFF);

    return false;
}

bool SharpSM83::IT_SUB(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) - fetch_info.data;

    this->write_reg((SharpSM83::reg_type) fetch_info.dest, val & 0xFFFF);

    return false;
}

bool SharpSM83::IT_SBC(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) - fetch_info.data;
    u8 carry = (this->read_reg(RT_F) & flags::c) != 0;

    this->write_reg((SharpSM83::reg_type) fetch_info.dest, (val - carry) & 0xFFFF);

    return false;
}

bool SharpSM83::IT_AND(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) & fetch_info.data;
    this->write_reg((SharpSM83::reg_type) fetch_info.dest, val & 0xFFFF);
    return false;
}

bool SharpSM83::IT_XOR(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) ^ fetch_info.data;
    this->write_reg((SharpSM83::reg_type) fetch_info.dest, val & 0xFFFF);
    return false;
}

bool SharpSM83::IT_OR(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) | fetch_info.data;
    this->write_reg((SharpSM83::reg_type) fetch_info.dest, val & 0xFFFF);
    return false;
}

bool SharpSM83::IT_CP(){
    u32 val = read_reg((SharpSM83::reg_type) fetch_info.inst.reg_1) - fetch_info.data;
    return false;
}

bool SharpSM83::IT_POP(){
    return false;
}

bool SharpSM83::IT_JP(){return false;}
bool SharpSM83::IT_PUSH(){return false;}
bool SharpSM83::IT_RET(){return false;}
bool SharpSM83::IT_CB(){return false;}
bool SharpSM83::IT_CALL(){return false;}
bool SharpSM83::IT_RETI(){return false;}
bool SharpSM83::IT_LDH(){return false;}
bool SharpSM83::IT_JPHL(){return false;}
bool SharpSM83::IT_DI(){return false;}
bool SharpSM83::IT_EI(){return false;}
bool SharpSM83::IT_RST(){return false;}
bool SharpSM83::IT_ERR(){return false;}
//CB instructions...
bool SharpSM83::IT_RLC(){return false;}
bool SharpSM83::IT_RRC(){return false;}
bool SharpSM83::IT_RL(){return false;}
bool SharpSM83::IT_RR(){return false;}
bool SharpSM83::IT_SLA(){return false;}
bool SharpSM83::IT_SRA(){return false;}
bool SharpSM83::IT_SWAP(){return false;}
bool SharpSM83::IT_SRL(){return false;}
bool SharpSM83::IT_BIT(){return false;}
bool SharpSM83::IT_RES(){return false;}
bool SharpSM83::IT_SET(){return false;}

