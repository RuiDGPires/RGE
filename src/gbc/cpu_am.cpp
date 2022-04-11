/*********************************************//**
 * \file cpu_am.hpp
 * \brief Implementation of addressing modes
 ************************************************/
#include "cpu.hpp"

void SharpSM83::AM_IMP(){
    if (fetch_info.inst.type == &SharpSM83::IT_STOP)
        regs[PC]++;

}
void SharpSM83::AM_R_D16(){
    fetch_info.dest = fetch_info.inst.reg_1;
    fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
    fetch_info.data |= (this->read(regs[PC]++) << 8) & 0xFF00;
    fetch_info.is_16_bit = true;
}
void SharpSM83::AM_D16(){
    fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
    fetch_info.data |= (this->read(regs[PC]++) << 8) & 0xFF00;
    fetch_info.is_16_bit = true;
}
void SharpSM83::AM_R_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.dest = fetch_info.inst.reg_1;
}
void SharpSM83::AM_MR_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);

    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_1);
    fetch_info.dest = fetch_info.inst.reg_1;

    fetch_info.is_16_bit = (fetch_info.inst.reg_1 & 0xFF) && (fetch_info.inst.reg_1 & 0xFF00);
}
void SharpSM83::AM_R_A8(){
    fetch_info.data = (this->read(regs[PC]++)) | 0xFF00;
    fetch_info.data = this->bus->read(fetch_info.data);
    fetch_info.dest = fetch_info.inst.reg_1;
}
void SharpSM83::AM_R_D8(){
    fetch_info.dest = fetch_info.inst.reg_1;
    fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
}
void SharpSM83::AM_D8(){
    fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
}
void SharpSM83::AM_R_MR(){
    fetch_info.data = this->bus->read(this->read_reg(fetch_info.inst.reg_2));

    fetch_info.dest = fetch_info.inst.reg_1;
}
void SharpSM83::AM_R_MHLI(){
    fetch_info.data = this->read(this->read_reg(RT_HL));
    this->write_reg(RT_HL, this->read_reg(RT_HL) + 1);
    fetch_info.dest = fetch_info.inst.reg_1;
}
void SharpSM83::AM_R_MHLD(){
    fetch_info.data = this->read(this->read_reg(RT_HL));
    this->write_reg(RT_HL, this->read_reg(RT_HL) - 1);
    fetch_info.dest = fetch_info.inst.reg_1;
}
void SharpSM83::AM_MHLI_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
    this->write_reg(RT_HL, this->read_reg(RT_HL) + 1);
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_MHLD_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
    this->write_reg(RT_HL, this->read_reg(RT_HL) - 1);
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_A8_R(){
    fetch_info.dest = this->bus->read(this->regs[PC]++) | 0xFF00;
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_HL_SPR(){
    fetch_info.data = this->read(this->regs[PC]++);
    fetch_info.dest = fetch_info.inst.reg_1;
    //fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_A16_R(){
    this->SharpSM83::AM_D16_R();
}
void SharpSM83::AM_D16_R(){
    fetch_info.data = this->read_reg(fetch_info.inst.reg_2);
    fetch_info.dest = (this->read(regs[PC]++)) & 0xFF;
    fetch_info.dest |= ((u32)(this->read(regs[PC]++)) << 8) & 0xFF00;
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_MR_D8(){
    fetch_info.data = this->read(this->regs[PC]++);
    fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_MR(){
    fetch_info.data = this->read(this->read_reg(fetch_info.inst.reg_1));
    fetch_info.dest = this->read_reg(fetch_info.inst.reg_1);
    fetch_info.is_dest_addr = true;
}
void SharpSM83::AM_R_A16(){
    fetch_info.data = (this->read(regs[PC]++)) & 0xFF;
    fetch_info.data |= (this->read(regs[PC]++) << 8) & 0xFF00;
    fetch_info.data = this->read(fetch_info.data);
    fetch_info.dest = fetch_info.inst.reg_1;
    fetch_info.is_16_bit = true;
}
