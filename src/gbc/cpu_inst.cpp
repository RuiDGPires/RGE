#include "cpu.hpp"
#include "../common/assert.hpp"

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
        this->write_reg((reg_type) fetch_info.dest, fetch_info.data);

    return false;
}

bool SharpSM83::IT_INC(){
    u16 val = fetch_info.data + 1;
    if (!fetch_info.is_16_bit)
        val = (u8) val;

    if (fetch_info.is_dest_addr)
        this->write(fetch_info.dest, val);
    else
        this->write_reg((reg_type) fetch_info.dest, val);

    if ((fetch_info.op_code & 0x03) != 0x03)
        this->set_flags(val == 0, 1, (val & 0x0F) == 0, NA);

    return false;
}

bool SharpSM83::IT_DEC(){
    u16 val = fetch_info.data - 1;

    if (!fetch_info.is_16_bit)
        val = (u8) val;

    if (fetch_info.is_dest_addr)
        this->write(fetch_info.dest, val);
    else
        this->write_reg((reg_type) fetch_info.dest, val);

    this->set_flags(val == 0, 1, (val & 0x0F) == 0x0F, NA);

    return false;
}

bool SharpSM83::IT_RLCA(){
    fetch_info.dest = RT_A;
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RLC(true);
}

bool SharpSM83::IT_ADD(){
    ASSERT(!(fetch_info.is_dest_addr), "Dest must be a register");

    bool is_16_bit = ((fetch_info.inst.reg_1 & 0xFFFF) ^ 0xFFFF) == 0;

    u8 z, h, c;
    u32 val;

    if (fetch_info.inst.reg_1 == RT_SP){
        val = read_reg((reg_type) fetch_info.inst.reg_1) + (u8) fetch_info.data;
    
        z = 0;
        h = (read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) + (fetch_info.data & 0xF) >= 0x10;
        c = val >= 0x100;
        goto end;
    }

    val = read_reg((reg_type) fetch_info.inst.reg_1) + fetch_info.data;

    if (is_16_bit){
        z = (val & 0xFFFF) == 0;
        h = (read_reg((reg_type) fetch_info.inst.reg_1) & 0xFF) + (fetch_info.data & 0xFF) >= 0x1000;
        c = val >= 0x10000;
    }else{
        z = (val & 0xFF) == 0;
        h = (read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) + (fetch_info.data & 0xF) >= 0x10;
        c = val >= 0x100;
    }

    if (fetch_info.inst.reg_1 == RT_HL)
        z = NA;
end:
    this->set_flags(z, 0, h, c);
    this->write_reg((reg_type) fetch_info.dest, val & 0xFFFF);

    return false;
}

bool SharpSM83::IT_RRCA(){
    fetch_info.dest = RT_A;
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RRC(true);
}

bool SharpSM83::IT_STOP(){
    this->running = false;
    this->halted = false;
    return false;
}

bool SharpSM83::IT_RLA(){
    fetch_info.dest = RT_A;
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RL(true);
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
        case CT_NONE:
            return true;
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
    fetch_info.data = this->regs[PC] + (i8) fetch_info.data;
    return this->jump();
}

bool SharpSM83::IT_RRA(){
    fetch_info.dest = RT_A;
    fetch_info.data = this->read_reg(RT_A);

    return this->IT_RR(true);
}

bool SharpSM83::IT_DAA(){
    u8 u = 0;
    int fc = 0;
    
    u8 flags_ = read_reg(RT_F);

    u8 n = (flags_ & flags::n) != 0;
    u8 h = (flags_ & flags::h) != 0;
    u8 c = (flags_ & flags::c) != 0;

    if (h || (!n && (read_reg(RT_A) & 0xF) > 9))
        u = 6;

    if (c || (!n && read_reg(RT_A) > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    write_reg(RT_A, read_reg(RT_A) + n ? -u : u);

    this->set_flags(read_reg(RT_A) == 0, NA, 0, fc);
    return false;
}

bool SharpSM83::IT_CPL(){
    this->write_reg(RT_A, ~this->read(RT_A));
    this->set_flags(NA, 1, 1, NA);
    return false;
}

bool SharpSM83::IT_SCF(){
    // Set carry flag
    this->set_flags(NA, 0, 0, 1);
    return false;
}

bool SharpSM83::IT_CCF(){
    this->set_flags(NA, 0, 0, (this->read(RT_F) & flags::c) ^ flags::c);
    return false;
}

bool SharpSM83::IT_HALT(){
    this->halted = true;
    this->regs[PC]--;
    return false;
}

bool SharpSM83::IT_ADC(){
    u32 val = read_reg((reg_type) fetch_info.inst.reg_1) + fetch_info.data;
    u8 carry = (this->read_reg(RT_F) & flags::c) != 0;

    val += carry;

    u8 h = (read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) + (fetch_info.data & 0xF);

    this->write_reg((reg_type) fetch_info.dest, val & 0xFF);

    this->set_flags(val == 0, 0, h, val > 0xFF);

    return false;
}

bool SharpSM83::IT_SUB(){
    u32 val = read_reg((reg_type) fetch_info.inst.reg_1) - fetch_info.data;

    u8 h = (read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) - (fetch_info.data & 0xF);

    this->write_reg((reg_type) fetch_info.dest, val & 0xFFFF);

    this->set_flags(val == 0, 1, h, ((int) val) < 0);

    return false;
}

bool SharpSM83::IT_SBC(){
    u8 val = read_reg((reg_type) fetch_info.inst.reg_1) - fetch_info.data;
    u8 carry = (this->read_reg(RT_F) & flags::c) != 0;

    u8 h = (int)((read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) - (fetch_info.data & 0xF) - carry) < 0;

    this->set_flags(val - carry == 0, 1, h, (int)(val - carry) < 0);
    this->write_reg((reg_type) fetch_info.dest, (val - carry) & 0xFFFF);

    return false;
}

bool SharpSM83::IT_AND(){
    u16 val = read_reg((reg_type) fetch_info.inst.reg_1) & fetch_info.data;
    this->write_reg((reg_type) fetch_info.dest, val & 0xFFFF);
    this->set_flags(val == 0, 0, 1, 0);
    return false;
}

bool SharpSM83::IT_XOR(){
    u32 val = read_reg((reg_type) fetch_info.inst.reg_1) ^ fetch_info.data;
    this->write_reg((reg_type) fetch_info.dest, val & 0xFFFF);
    this->set_flags(val == 0, 0, 0, 0);
    return false;
}

bool SharpSM83::IT_OR(){
    u32 val = read_reg((reg_type) fetch_info.inst.reg_1) | fetch_info.data;
    this->write_reg((reg_type) fetch_info.dest, val & 0xFFFF);
    this->set_flags(val == 0, 0, 0, 0);
    return false;
}

bool SharpSM83::IT_CP(){
    u32 val = read_reg((reg_type) fetch_info.inst.reg_1) - fetch_info.data;
    u8 h = (int)((read_reg((reg_type) fetch_info.inst.reg_1) & 0xF) - (fetch_info.data & 0xF));

    this->set_flags(val == 0, 1, h, ((int) val) < 0);

    return false;
}

bool SharpSM83::IT_POP(){
    u16 val = 0;
    val |= this->pop();
    val |= (this->pop() << 8) & 0xFF00;

    this->write_reg((reg_type) fetch_info.inst.reg_1, val);

    if (fetch_info.inst.reg_1 == RT_AF)
        write_reg(RT_AF, val & 0xFFF0);

    return false;
}

bool SharpSM83::IT_JP(){
    return this->jump();
}

bool SharpSM83::IT_PUSH(){
    u8 val = (read_reg(fetch_info.inst.reg_1) >> 8) & 0xFF;
    this->push(val);
    val = read_reg(fetch_info.inst.reg_1) & 0xFF;
    this->push(val);
    return false;
}

bool SharpSM83::IT_RET(){
    if (eval_condition()){
        u16 val = this->pop() & 0x00FF;
        val |= (this->pop() << 8) & 0xFF00;

        this->regs[PC] = val;
        return true;
    }

    return false;
}

bool SharpSM83::IT_CB(){
    u8 op = fetch_info.data;
    
    u8 reg_bits = op & 0b111; 
    bool is_left_reg = reg_bits & 1; 
    reg_type reg = (reg_type) (((reg_bits >> 1) << 16) | (0xFF << 8 * is_left_reg));

    u8 bit = (op >> 3) & 0b111;
    u8 bit_op = (op >> 6) & 0b11;
    u8 val = read_reg(reg);

    switch(bit_op){
        case 1: // Bit
            this->set_flags(!(val & (1 << bit)), 0, 1, NA);
            return false;
        case 2: // RST
            write_reg(reg, val & ~(1 << bit));
            return false;
        case 3: // SET
            write_reg(reg, val | (1 << bit));
            return false;
    }

    switch (bit){
        case 1:
            return IT_RRC();
        case 2:
            return IT_RL();
        case 3:
            return IT_RR();
        case 4:
            return IT_SLA();
        case 5:
            return IT_SRA();
        case 6:
            return IT_SWAP();
        case 7:
            return IT_SRL();
    }

    return false;
}

bool SharpSM83::IT_CALL(){
    return this->jump(true);
}

bool SharpSM83::IT_RETI(){
    this->IME = true;
    return this->IT_RET();
}

bool SharpSM83::IT_LDH(){
    if (fetch_info.inst.reg_1 == RT_A) {
        write_reg(fetch_info.inst.reg_1, this->read(0xFF00 | fetch_info.data));
    } else {
        this->write(fetch_info.dest, fetch_info.data);
    }

    return false;
}

bool SharpSM83::IT_DI(){
    this->IME = false;
    return false;
}

bool SharpSM83::IT_EI(){
    this->ei = true; // delay the effect by one instruction
    return false;
}

bool SharpSM83::IT_RST(){
    fetch_info.dest = fetch_info.inst.param;
    return this->jump(true);
}

//CB instructions...
bool SharpSM83::IT_RLC(bool clear_z){
    u8 carry = (fetch_info.data & (1 << 7)) != 0;
    u8 res = (fetch_info.data << 1) | carry;

    set_flags((res == 0) & !clear_z, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_RRC(bool clear_z){
    u8 carry = fetch_info.data & 0x1;
    u8 res = (fetch_info.data >> 1) | (carry << 7);

    set_flags((res == 0) & !clear_z, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_RL(bool clear_z){
    u8 carry = (fetch_info.data & (1 << 7)) != 0;
    u8 res = (fetch_info.data << 1) | ((read_reg(RT_F) & flags::c) != 0);

    set_flags((res == 0) & !clear_z, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_RR(bool clear_z){
    u8 carry = fetch_info.data & 0x1;
    u8 res = (fetch_info.data >> 1) | (((read_reg(RT_F) & flags::c) != 0) << 7);

    set_flags((res == 0) & !clear_z, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_SLA(){
    u8 carry = (fetch_info.data & (1 << 7)) != 0;

    u8 res = (fetch_info.data << 1);

    set_flags(res == 0, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_SRA(){
    u8 b = (fetch_info.data & (1 << 7));
    u8 carry = fetch_info.data & 0x1;

    u8 res = (fetch_info.data >> 1) | b;

    set_flags(res == 0, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_SWAP(){
    u8 res = ((fetch_info.data & 0xF0) >> 4) | ((fetch_info.data & 0x0F) << 4);

    set_flags(res == 0, 0, 0, 0);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}

bool SharpSM83::IT_SRL(){
    u8 carry = fetch_info.data & 0x1;

    u8 res = fetch_info.data >> 1;

    set_flags(res == 0, 0, 0, carry);

    write_reg((reg_type) fetch_info.dest, res);
    return false;
}
