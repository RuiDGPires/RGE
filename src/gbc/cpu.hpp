#pragma once

#include "bus.hpp"
#include "../common/defs.hpp"

#define SP 4
#define PC 5

class Bus;

class SharpSM83{
    private:
        enum reg_type{
            RT_NONE = 0, 
            RT_A = 0x0FF00,
            RT_F = 0x000FF,
            RT_B = 0x1FF00,
            RT_C = 0x100FF,
            RT_D = 0x2FF00,
            RT_E = 0x200FF,
            RT_H = 0x3FF00,
            RT_L = 0x300FF,
            RT_AF = 0x0FFFF,
            RT_BC = 0x1FFFF,
            RT_DE = 0x2FFFF,
            RT_HL = 0x3FFFF,
            RT_SP = 0x4FFFF,
            RT_PC = 0x5FFFF,
        };
        void write_reg(reg_type reg_t, u16 data);
        u16 read_reg(reg_type reg_t);

        void push(u8 data);
        void push(u16 data);
        u8 pop();

        bool jump(bool save_pc);
        bool eval_condition();

        bool IT_NONE();
        bool IT_NOP();
        bool IT_LD();
        bool IT_INC();
        bool IT_DEC();
        bool IT_RLCA();
        bool IT_ADD();
        bool IT_RRCA();
        bool IT_STOP();
        bool IT_RLA();
        bool IT_JR();
        bool IT_RRA();
        bool IT_DAA();
        bool IT_CPL();
        bool IT_SCF();
        bool IT_CCF();
        bool IT_HALT();
        bool IT_ADC();
        bool IT_SUB();
        bool IT_SBC();
        bool IT_AND();
        bool IT_XOR();
        bool IT_OR();
        bool IT_CP();
        bool IT_POP();
        bool IT_JP();
        bool IT_PUSH();
        bool IT_RET();
        bool IT_CB();
        bool IT_CALL();
        bool IT_RETI();
        bool IT_LDH();
        bool IT_JPHL();
        bool IT_DI();
        bool IT_EI();
        bool IT_RST();
        bool IT_ERR();
        //CB instructions...
        bool IT_RLC();
        bool IT_RRC();
        bool IT_RL();
        bool IT_RR();
        bool IT_SLA();
        bool IT_SRA();
        bool IT_SWAP();
        bool IT_SRL();
        bool IT_BIT();
        bool IT_RES();
        bool IT_SET();

        //Addressing modes
        void AM_IMP();
        void AM_R_D16();
        void AM_R_R();
        void AM_MR_R();
        void AM_R();
        void AM_R_D8();
        void AM_R_MR();
        void AM_R_MHLI();
        void AM_R_MHLD();
        void AM_MHLI_R();
        void AM_MHLD_R();
        void AM_R_A8();
        void AM_A8_R();
        void AM_MHL_SPR();
        void AM_D16();
        void AM_D8();
        void AM_D16_R();
        void AM_MR_D8();
        void AM_MR();
        void AM_A16_R();
        void AM_R_A16();

    
        enum cond_type{
            CT_NONE, CT_NZ, CT_Z, CT_NC, CT_C
        };

        struct instruction{
            u8 cycles;
            const char *str;
            bool (SharpSM83::*type)(void);
            void (SharpSM83::*mode)(void);
            reg_type reg_1;
            reg_type reg_2;
            cond_type cond;
            u8 extra_cycles;
            u8 param;
        };
        
        using a = SharpSM83;
        const instruction lookup[0x100] = {
            {4, "NOP", &a::IT_NOP, &a::AM_IMP},
            {12, "LD", &a::IT_LD, &a::AM_R_D16, RT_BC},
            {8, "LD", &a::IT_LD, &a::AM_MR_R, RT_BC, RT_A},
            {8, "INC", &a::IT_INC, &a::AM_R, RT_BC},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_B},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_B},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_B},
            {4, "RLCA", &a::IT_RLCA, &a::AM_IMP},
            {20, "LD", &a::IT_LD, &a::AM_A16_R, RT_NONE, RT_SP},
            {8, "ADD", &a::IT_ADD, &a::AM_R_R, RT_HL, RT_BC},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_A, RT_BC},
            {8, "DEC", &a::IT_DEC, &a::AM_R, RT_BC},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_C},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_C},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_C},
            {4, "RRCA", &a::IT_RRCA, &a::AM_IMP},
            
            {4, "STOP", &a::IT_STOP},
            {12, "LD", &a::IT_LD, &a::AM_R_D16, RT_DE},
            {8, "LD", &a::IT_LD, &a::AM_MR_R, RT_DE, RT_A},
            {8, "INC", &a::IT_INC, &a::AM_R, RT_DE},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_D},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_D},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_D},
            {4, "RLA", &a::IT_RLA},
            {12, "JR", &a::IT_JR, &a::AM_D8},
            {8, "ADD", &a::IT_ADD, &a::AM_R_R, RT_HL, RT_DE},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_A, RT_DE},
            {8, "DEC", &a::IT_DEC, &a::AM_R, RT_DE},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_E},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_E},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_E},
            {4, "RRA", &a::IT_RRA},

            {8, "JR", &a::IT_JR, &a::AM_D8, RT_NONE, RT_NONE, CT_NZ, 4},
            {12, "LD", &a::IT_LD, &a::AM_R_D16, RT_HL},
            {8, "LD", &a::IT_LD, &a::AM_MHLI_R, RT_HL, RT_A},
            {8, "INC", &a::IT_INC, &a::AM_R, RT_HL},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_H},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_H},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_H},
            {4, "DAA", &a::IT_DAA},
            {8, "JR", &a::IT_JR, &a::AM_D8, RT_NONE, RT_NONE, CT_Z, 4},
            {8, "ADD", &a::IT_ADD, &a::AM_R_R, RT_HL, RT_HL},
            {8, "LD", &a::IT_LD, &a::AM_R_MHLI, RT_A, RT_HL},
            {8, "DEC", &a::IT_DEC, &a::AM_R, RT_HL},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_L},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_L},
            {4, "CPL", &a::IT_CPL, &a::AM_IMP},

            {8, "JR", &a::IT_JR, &a::AM_D8, RT_NONE, RT_NONE, CT_NC, 4},
            {12, "LD", &a::IT_LD, &a::AM_R_D16, RT_SP},
            {8, "LD", &a::IT_LD, &a::AM_MHLD_R, RT_HL, RT_A},
            {8, "INC", &a::IT_INC, &a::AM_R, RT_SP},
            {12, "INC", &a::IT_INC, &a::AM_MR, RT_HL},
            {12, "DEC", &a::IT_DEC, &a::AM_MR, RT_HL},
            {12, "LD", &a::IT_LD, &a::AM_MR_D8, RT_HL},
            {4, "SCF", &a::IT_SCF},
            {8, "JR", &a::IT_JR, &a::AM_D8, RT_NONE, RT_NONE, CT_C, 4},
            {8, "ADD", &a::IT_ADD, &a::AM_R_R, RT_HL, RT_SP},
            {8, "LD", &a::IT_LD, &a::AM_R_MHLD, RT_A, RT_HL},
            {8, "DEC", &a::IT_DEC, &a::AM_R, RT_SP},
            {4, "INC", &a::IT_INC, &a::AM_R, RT_A},
            {4, "DEC", &a::IT_DEC, &a::AM_R, RT_A},
            {8, "LD", &a::IT_LD, &a::AM_R_D8, RT_A},
            {4, "CCF", &a::IT_CCF, &a::AM_IMP},

            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_B, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_B, RT_A},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_C, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R, RT_C, RT_A},

            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_D, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_D, RT_A},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_E, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_E, RT_A},
            
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_H, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_H, RT_A},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_L, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_L, RT_A},

            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_B},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_C},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_D},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_E},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_H},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_L},
            {4, "HALT", &a::IT_HALT},
            {8, "LD", &a::IT_LD, &a::AM_MR_R,  RT_HL, RT_A},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_B},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_C},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_D},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_E},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_H},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_L},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_A, RT_HL},
            {4, "LD", &a::IT_LD, &a::AM_R_R,  RT_A, RT_A},
            
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_B},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_C},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_D},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_E},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_H},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_L},
            {8, "ADD", &a::IT_ADD, &a::AM_R_MR, RT_A, RT_HL},
            {4, "ADD", &a::IT_ADD, &a::AM_R_R, RT_A, RT_A},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_B},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_C},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_D},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_E},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_H},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_L},
            {8, "ADC", &a::IT_ADC, &a::AM_R_MR, RT_A, RT_HL},
            {4, "ADC", &a::IT_ADC, &a::AM_R_R, RT_A, RT_A},

            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_B},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_C},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_D},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_E},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_H},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_L},
            {8, "SUB", &a::IT_SUB, &a::AM_R_MR, RT_A, RT_HL},
            {4, "SUB", &a::IT_SUB, &a::AM_R_R, RT_A, RT_A},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_B},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_C},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_D},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_E},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_H},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_L},
            {8, "SBC", &a::IT_SBC, &a::AM_R_MR, RT_A, RT_HL},
            {4, "SBC", &a::IT_SBC, &a::AM_R_R, RT_A, RT_A},

            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_B},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_C},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_D},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_E},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_H},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_L},
            {8, "AND", &a::IT_AND, &a::AM_R_MR, RT_A, RT_HL},
            {4, "AND", &a::IT_AND, &a::AM_R_R, RT_A, RT_A},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_B},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_C},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_D},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_E},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_H},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_L},
            {8, "XOR", &a::IT_XOR, &a::AM_R_MR, RT_A, RT_HL},
            {4, "XOR", &a::IT_XOR, &a::AM_R_R, RT_A, RT_A},

            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_B},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_C},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_D},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_E},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_H},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_L},
            {8, "OR", &a::IT_OR, &a::AM_R_MR, RT_A, RT_HL},
            {4, "OR", &a::IT_OR, &a::AM_R_R, RT_A, RT_A},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_B},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_C},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_D},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_E},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_H},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_L},
            {8, "CP", &a::IT_CP, &a::AM_R_MR, RT_A, RT_HL},
            {4, "CP", &a::IT_CP, &a::AM_R_R, RT_A, RT_A},

            {8, "RET", &a::IT_RET, &a::AM_IMP, RT_NONE, RT_NONE, CT_NZ, 12},
            {12, "POP", &a::IT_POP, &a::AM_R, RT_BC},
            {12, "JP", &a::IT_JP, &a::AM_D16, RT_NONE, RT_NONE, CT_NZ, 4},
            {16, "JP", &a::IT_JP, &a::AM_D16},
            {12, "CALL", &a::IT_CALL, &a::AM_D16, RT_NONE, RT_NONE, CT_NZ, 12},
            {16, "PUSH", &a::IT_PUSH, &a::AM_R, RT_BC},
            {8, "ADD", &a::IT_ADD, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x00},
            {8, "RET", &a::IT_RET, &a::AM_IMP, RT_NONE, RT_NONE, CT_Z, 12},
            {16, "RET", &a::IT_RET},
            {12, "JP", &a::IT_JP, &a::AM_D16, RT_NONE, RT_NONE, CT_Z, 4},
            {4, "CB", &a::IT_CB, &a::AM_D8},
            {12, "CALL", &a::IT_CALL, &a::AM_D16, RT_NONE, RT_NONE, CT_Z, 12},
            {24, "CALL", &a::IT_CALL, &a::AM_D16},
            {8, "ADC", &a::IT_ADC, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x08},

            {8, "RET", &a::IT_RET, &a::AM_IMP, RT_NONE, RT_NONE, CT_NC, 12},
            {12, "POP", &a::IT_POP, &a::AM_R, RT_DE},
            {12, "JP", &a::IT_JP, &a::AM_D16, RT_NONE, RT_NONE, CT_NC, 4},
            {0, "---", &a::IT_NONE},
            {12, "CALL", &a::IT_CALL, &a::AM_D16, RT_NONE, RT_NONE, CT_NC, 12},
            {16, "PUSH", &a::IT_PUSH, &a::AM_R, RT_DE},
            {8, "SUB", &a::IT_SUB, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x10},
            {8, "RET", &a::IT_RET, &a::AM_IMP, RT_NONE, RT_NONE, CT_C, 12},
            {16, "RETI", &a::IT_RETI},
            {12, "JP", &a::IT_JP, &a::AM_D16, RT_NONE, RT_NONE, CT_C, 4},
            {0, "---", &a::IT_NONE},
            {12, "CALL", &a::IT_CALL, &a::AM_D16, RT_NONE, RT_NONE, CT_C, 12},
            {0, "---", &a::IT_NONE},
            {8, "SBC", &a::IT_SBC, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x18},
            
            {12, "LDH", &a::IT_LDH, &a::AM_A8_R, RT_NONE, RT_A},
            {12, "POP", &a::IT_POP, &a::AM_R, RT_HL},
            {8, "LD", &a::IT_LD, &a::AM_MR_R, RT_C, RT_A},
            {0, "---", &a::IT_NONE},
            {0, "---", &a::IT_NONE},
            {16, "PUSH", &a::IT_PUSH, &a::AM_R, RT_HL},
            {8, "AND", &a::IT_AND, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x20},
            {16, "ADD", &a::IT_ADD, &a::AM_R_D8, RT_SP},
            {4, "JP", &a::IT_JP, &a::AM_R, RT_HL},
            {16, "LD", &a::IT_LD, &a::AM_A16_R, RT_NONE, RT_A},
            {0, "---", &a::IT_NONE},
            {0, "---", &a::IT_NONE},
            {0, "---", &a::IT_NONE},
            {8, "XOR", &a::IT_XOR, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x28},
            
            {12, "LDH", &a::IT_LDH, &a::AM_R_A8, RT_A},
            {12, "POP", &a::IT_POP, &a::AM_R, RT_AF},
            {8, "LD", &a::IT_LD, &a::AM_R_MR, RT_A, RT_C},
            {4, "DI", &a::IT_DI},
            {0, "---", &a::IT_NONE},
            {16, "PUSH", &a::IT_PUSH, &a::AM_R, RT_AF},
            {8, "OR", &a::IT_OR, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x30},
            {12, "LD", &a::IT_LD, &a::AM_MHL_SPR, RT_HL, RT_SP},
            {8, "LD", &a::IT_LD, &a::AM_R_R, RT_SP, RT_HL},
            {16, "LD", &a::IT_LD, &a::AM_R_A16, RT_A},
            {4, "EI", &a::IT_EI},
            {0, "---", &a::IT_NONE},
            {0, "---", &a::IT_NONE},
            {8, "CP", &a::IT_CP, &a::AM_R_D8, RT_A},
            {16, "RST", &a::IT_RST, &a::AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x38},
        };

        struct fetch_info_t{
            u8 op_code;
            instruction inst;
            bool is_dest_addr;

            u16 data, dest;
        };

        enum flags{
            z = 1 << 7,
            n = 1 << 6,
            h = 1 << 5,
            c = 1 << 4,
        };


        u8 cycles = 0;
        fetch_info_t fetch_info = {0, {0, "---", &a::IT_NONE}, false, 0, 0};

        u16 regs[6];

        Bus *bus;
        bool running;

        const instruction get_instruction(u8 opcode);
        const char *instruction_name(instruction inst);

        void write(u16 addr, u8 data);
        u8 read(u16 addr);


  
    public:
        SharpSM83();
        ~SharpSM83();

        void connect_bus(Bus *);
        void clock();
        void reset();

        void run();
};
