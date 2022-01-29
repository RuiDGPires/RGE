#include "instructions.hpp"

const instruction instruction_set[0x100] = {
    {IT_NOP, AM_IMP}, {IT_LD, AM_R_D16, RT_BC}, {IT_LD, AM_MR_R, RT_BC, RT_A}, {IT_INC, AM_R, RT_BC}, {IT_INC, AM_R, RT_B}, {IT_DEC, AM_R, RT_B}, {IT_LD, AM_R_D8, RT_B}, {IT_RLCA}, {IT_LD, AM_A16_R, RT_NONE, RT_SP}, {IT_ADD, AM_R_R, RT_HL, RT_BC}, {IT_LD, AM_R_MR, RT_A, RT_BC}, {IT_DEC, AM_R, RT_BC}, {IT_INC, AM_R, RT_C}, {IT_DEC, AM_R, RT_C}, {IT_LD, AM_R_D8, RT_C}, {IT_RRCA},

     {IT_STOP}, {IT_LD, AM_R_D16, RT_DE}, {IT_LD, AM_MR_R, RT_DE, RT_A}, {IT_INC, AM_R, RT_DE}, {IT_INC, AM_R, RT_D}, {IT_DEC, AM_R, RT_D}, {IT_LD, AM_R_D8, RT_D}, {IT_RLA}, {IT_JR, AM_D8}, {IT_ADD, AM_R_R, RT_HL, RT_DE}, {IT_LD, AM_R_MR, RT_A, RT_DE}, {IT_DEC, AM_R, RT_DE}, {IT_INC, AM_R, RT_E}, {IT_DEC, AM_R, RT_E}, {IT_LD, AM_R_D8, RT_E}, {IT_RRA},

     {IT_JR, AM_D8, RT_NONE, RT_NONE, CT_NZ}, {IT_LD, AM_R_D16, RT_HL}, {IT_LD, AM_HLI_R, RT_HL, RT_A}, {IT_INC, AM_R, RT_HL}, {IT_INC, AM_R, RT_H}, {IT_DEC, AM_R, RT_H}, {IT_LD, AM_R_D8, RT_H}, {IT_DAA}, {IT_JR, AM_D8, RT_NONE, RT_NONE, CT_Z}, {IT_ADD, AM_R_R, RT_HL, RT_HL}, {IT_LD, AM_R_HLI, RT_A, RT_HL}, {IT_DEC, AM_R, RT_HL}, {IT_INC, AM_R, RT_L}, {IT_DEC, AM_R, RT_L}, {IT_LD, AM_R_D8, RT_L}, {IT_CPL},

     {IT_JR, AM_D8, RT_NONE, RT_NONE, CT_NC}, {IT_LD, AM_R_D16, RT_SP}, {IT_LD, AM_HLD_R, RT_HL, RT_A}, {IT_INC, AM_R, RT_SP}, {IT_INC, AM_MR, RT_HL}, {IT_DEC, AM_MR, RT_HL}, {IT_LD, AM_MR_D8, RT_HL}, {IT_SCF}, {IT_JR, AM_D8, RT_NONE, RT_NONE, CT_C}, {IT_ADD, AM_R_R, RT_HL, RT_SP}, {IT_LD, AM_R_HLD, RT_A, RT_HL}, {IT_DEC, AM_R, RT_SP}, {IT_INC, AM_R, RT_A}, {IT_DEC, AM_R, RT_A}, {IT_LD, AM_R_D8, RT_A}, {IT_CCF},

     {IT_LD, AM_R_R, RT_B, RT_B}, {IT_LD, AM_R_R, RT_B, RT_C}, {IT_LD, AM_R_R, RT_B, RT_D}, {IT_LD, AM_R_R, RT_B, RT_E}, {IT_LD, AM_R_R, RT_B, RT_H}, {IT_LD, AM_R_R, RT_B, RT_L}, {IT_LD, AM_R_MR, RT_B, RT_HL}, {IT_LD, AM_R_R, RT_B, RT_A}, {IT_LD, AM_R_R, RT_C, RT_B}, {IT_LD, AM_R_R, RT_C, RT_C}, {IT_LD, AM_R_R, RT_C, RT_D}, {IT_LD, AM_R_R, RT_C, RT_E}, {IT_LD, AM_R_R, RT_C, RT_H}, {IT_LD, AM_R_R, RT_C, RT_L}, {IT_LD, AM_R_MR, RT_C, RT_HL}, {IT_LD, AM_R_R, RT_C, RT_A},

     {IT_LD, AM_R_R,  RT_D, RT_B}, {IT_LD, AM_R_R,  RT_D, RT_C}, {IT_LD, AM_R_R,  RT_D, RT_D}, {IT_LD, AM_R_R,  RT_D, RT_E}, {IT_LD, AM_R_R,  RT_D, RT_H}, {IT_LD, AM_R_R,  RT_D, RT_L}, {IT_LD, AM_R_MR, RT_D, RT_HL}, {IT_LD, AM_R_R,  RT_D, RT_A}, {IT_LD, AM_R_R,  RT_E, RT_B}, {IT_LD, AM_R_R,  RT_E, RT_C}, {IT_LD, AM_R_R,  RT_E, RT_D}, {IT_LD, AM_R_R,  RT_E, RT_E}, {IT_LD, AM_R_R,  RT_E, RT_H}, {IT_LD, AM_R_R,  RT_E, RT_L}, {IT_LD, AM_R_MR, RT_E, RT_HL}, {IT_LD, AM_R_R,  RT_E, RT_A},

     {IT_LD, AM_R_R,  RT_H, RT_B}, {IT_LD, AM_R_R,  RT_H, RT_C}, {IT_LD, AM_R_R,  RT_H, RT_D}, {IT_LD, AM_R_R,  RT_H, RT_E}, {IT_LD, AM_R_R,  RT_H, RT_H}, {IT_LD, AM_R_R,  RT_H, RT_L}, {IT_LD, AM_R_MR, RT_H, RT_HL}, {IT_LD, AM_R_R,  RT_H, RT_A}, {IT_LD, AM_R_R,  RT_L, RT_B}, {IT_LD, AM_R_R,  RT_L, RT_C}, {IT_LD, AM_R_R,  RT_L, RT_D}, {IT_LD, AM_R_R,  RT_L, RT_E}, {IT_LD, AM_R_R,  RT_L, RT_H}, {IT_LD, AM_R_R,  RT_L, RT_L}, {IT_LD, AM_R_MR, RT_L, RT_HL}, {IT_LD, AM_R_R,  RT_L, RT_A},

     {IT_LD, AM_MR_R,  RT_HL, RT_B}, {IT_LD, AM_MR_R,  RT_HL, RT_C}, {IT_LD, AM_MR_R,  RT_HL, RT_D}, {IT_LD, AM_MR_R,  RT_HL, RT_E}, {IT_LD, AM_MR_R,  RT_HL, RT_H}, {IT_LD, AM_MR_R,  RT_HL, RT_L}, {IT_HALT}, {IT_LD, AM_MR_R,  RT_HL, RT_A}, {IT_LD, AM_R_R,  RT_A, RT_B}, {IT_LD, AM_R_R,  RT_A, RT_C}, {IT_LD, AM_R_R,  RT_A, RT_D}, {IT_LD, AM_R_R,  RT_A, RT_E}, {IT_LD, AM_R_R,  RT_A, RT_H}, {IT_LD, AM_R_R,  RT_A, RT_L}, {IT_LD, AM_R_MR, RT_A, RT_HL}, {IT_LD, AM_R_R,  RT_A, RT_A},

     {IT_ADD, AM_R_R, RT_A, RT_B}, {IT_ADD, AM_R_R, RT_A, RT_C}, {IT_ADD, AM_R_R, RT_A, RT_D}, {IT_ADD, AM_R_R, RT_A, RT_E}, {IT_ADD, AM_R_R, RT_A, RT_H}, {IT_ADD, AM_R_R, RT_A, RT_L}, {IT_ADD, AM_R_MR, RT_A, RT_HL}, {IT_ADD, AM_R_R, RT_A, RT_A}, {IT_ADC, AM_R_R, RT_A, RT_B}, {IT_ADC, AM_R_R, RT_A, RT_C}, {IT_ADC, AM_R_R, RT_A, RT_D}, {IT_ADC, AM_R_R, RT_A, RT_E}, {IT_ADC, AM_R_R, RT_A, RT_H}, {IT_ADC, AM_R_R, RT_A, RT_L}, {IT_ADC, AM_R_MR, RT_A, RT_HL}, {IT_ADC, AM_R_R, RT_A, RT_A},

     {IT_SUB, AM_R_R, RT_A, RT_B}, {IT_SUB, AM_R_R, RT_A, RT_C}, {IT_SUB, AM_R_R, RT_A, RT_D}, {IT_SUB, AM_R_R, RT_A, RT_E}, {IT_SUB, AM_R_R, RT_A, RT_H}, {IT_SUB, AM_R_R, RT_A, RT_L}, {IT_SUB, AM_R_MR, RT_A, RT_HL}, {IT_SUB, AM_R_R, RT_A, RT_A}, {IT_SBC, AM_R_R, RT_A, RT_B}, {IT_SBC, AM_R_R, RT_A, RT_C}, {IT_SBC, AM_R_R, RT_A, RT_D}, {IT_SBC, AM_R_R, RT_A, RT_E}, {IT_SBC, AM_R_R, RT_A, RT_H}, {IT_SBC, AM_R_R, RT_A, RT_L}, {IT_SBC, AM_R_MR, RT_A, RT_HL}, {IT_SBC, AM_R_R, RT_A, RT_A},

     {IT_AND, AM_R_R, RT_A, RT_B}, {IT_AND, AM_R_R, RT_A, RT_C}, {IT_AND, AM_R_R, RT_A, RT_D}, {IT_AND, AM_R_R, RT_A, RT_E}, {IT_AND, AM_R_R, RT_A, RT_H}, {IT_AND, AM_R_R, RT_A, RT_L}, {IT_AND, AM_R_MR, RT_A, RT_HL}, {IT_AND, AM_R_R, RT_A, RT_A}, {IT_XOR, AM_R_R, RT_A, RT_B}, {IT_XOR, AM_R_R, RT_A, RT_C}, {IT_XOR, AM_R_R, RT_A, RT_D}, {IT_XOR, AM_R_R, RT_A, RT_E}, {IT_XOR, AM_R_R, RT_A, RT_H}, {IT_XOR, AM_R_R, RT_A, RT_L}, {IT_XOR, AM_R_MR, RT_A, RT_HL}, {IT_XOR, AM_R_R, RT_A, RT_A},

     {IT_OR, AM_R_R, RT_A, RT_B}, {IT_OR, AM_R_R, RT_A, RT_C}, {IT_OR, AM_R_R, RT_A, RT_D}, {IT_OR, AM_R_R, RT_A, RT_E}, {IT_OR, AM_R_R, RT_A, RT_H}, {IT_OR, AM_R_R, RT_A, RT_L}, {IT_OR, AM_R_MR, RT_A, RT_HL}, {IT_OR, AM_R_R, RT_A, RT_A}, {IT_CP, AM_R_R, RT_A, RT_B}, {IT_CP, AM_R_R, RT_A, RT_C}, {IT_CP, AM_R_R, RT_A, RT_D}, {IT_CP, AM_R_R, RT_A, RT_E}, {IT_CP, AM_R_R, RT_A, RT_H}, {IT_CP, AM_R_R, RT_A, RT_L}, {IT_CP, AM_R_MR, RT_A, RT_HL}, {IT_CP, AM_R_R, RT_A, RT_A},

     {IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_NZ}, {IT_POP, AM_R, RT_BC}, {IT_JP, AM_D16, RT_NONE, RT_NONE, CT_NZ}, {IT_JP, AM_D16}, {IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_NZ}, {IT_PUSH, AM_R, RT_BC}, {IT_ADD, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x00}, {IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_Z}, {IT_RET}, {IT_JP, AM_D16, RT_NONE, RT_NONE, CT_Z}, {IT_CB, AM_D8}, {IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_Z}, {IT_CALL, AM_D16}, {IT_ADC, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x08},

     {IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_NC}, {IT_POP, AM_R, RT_DE}, {IT_JP, AM_D16, RT_NONE, RT_NONE, CT_NC}, {IT_NONE}, {IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_NC}, {IT_PUSH, AM_R, RT_DE}, {IT_SUB, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x10}, {IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_C}, {IT_RETI}, {IT_JP, AM_D16, RT_NONE, RT_NONE, CT_C}, {IT_NONE}, {IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_C}, {IT_NONE}, {IT_SBC, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x18},

     {IT_LDH, AM_A8_R, RT_NONE, RT_A}, {IT_POP, AM_R, RT_HL}, {IT_LD, AM_MR_R, RT_C, RT_A}, {IT_NONE}, {IT_NONE}, {IT_PUSH, AM_R, RT_HL}, {IT_AND, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x20}, {IT_ADD, AM_R_D8, RT_SP}, {IT_JP, AM_R, RT_HL}, {IT_LD, AM_A16_R, RT_NONE, RT_A}, {IT_NONE}, {IT_NONE}, {IT_NONE}, {IT_XOR, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x28},

     {IT_LDH, AM_R_A8, RT_A}, {IT_POP, AM_R, RT_AF}, {IT_LD, AM_R_MR, RT_A, RT_C}, {IT_DI}, {IT_NONE}, {IT_PUSH, AM_R, RT_AF}, {IT_OR, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x30}, {IT_LD, AM_HL_SPR, RT_HL, RT_SP}, {IT_LD, AM_R_R, RT_SP, RT_HL}, {IT_LD, AM_R_A16, RT_A}, {IT_EI}, {IT_NONE}, {IT_NONE}, {IT_CP, AM_R_D8, RT_A}, {IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0x38},
};
