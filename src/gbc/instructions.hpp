#pragma once

#include "../common/defs.hpp"

enum instruction_type{
    IT_NONE,
    IT_NOP,
    IT_LD,
    IT_INC,
    IT_DEC,
    IT_RLCA,
    IT_ADD,
    IT_RRCA,
    IT_STOP,
    IT_RLA,
    IT_JR,
    IT_RRA,
    IT_DAA,
    IT_CPL,
    IT_SCF,
    IT_CCF,
    IT_HALT,
    IT_ADC,
    IT_SUB,
    IT_SBC,
    IT_AND,
    IT_XOR,
    IT_OR,
    IT_CP,
    IT_POP,
    IT_JP,
    IT_PUSH,
    IT_RET,
    IT_CB,
    IT_CALL,
    IT_RETI,
    IT_LDH,
    IT_JPHL,
    IT_DI,
    IT_EI,
    IT_RST,
    IT_ERR,
    //CB instructions...
    IT_RLC,
    IT_RRC,
    IT_RL,
    IT_RR,
    IT_SLA,
    IT_SRA,
    IT_SWAP,
    IT_SRL,
    IT_BIT,
    IT_RES,
    IT_SET
};

enum reg_type{
    RT_NONE,
    RT_A,
    RT_F,
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_AF,
    RT_BC,
    RT_DE,
    RT_HL,
    RT_SP,
    RT_PC
};

enum addr_mode{
    AM_IMP,
    AM_R_D16,
    AM_R_R,
    AM_MR_R,
    AM_R,
    AM_R_D8,
    AM_R_MR,
    AM_R_HLI,
    AM_R_HLD,
    AM_HLI_R,
    AM_HLD_R,
    AM_R_A8,
    AM_A8_R,
    AM_HL_SPR,
    AM_D16,
    AM_D8,
    AM_D16_R,
    AM_MR_D8,
    AM_MR,
    AM_A16_R,
    AM_R_A16
};

enum cond_type{
    CT_NONE, CT_NZ, CT_Z, CT_NC, CT_C
};

struct instruction{
    instruction_type type;
    addr_mode mode;
    reg_type reg_1;
    reg_type reg_2;
    cond_type cond;
    u8 param;
};
