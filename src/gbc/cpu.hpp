#pragma once

#include "bus.hpp"
#include "../common/defs.hpp"

class Bus;

class SharpSM83{
    public:
        const u8 PC = 5;
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

    private:
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

        enum addr_mode{
            AM_IMP,
            AM_R_D16,
            AM_R_R,
            AM_MR_R,
            AM_R,
            AM_R_D8,
            AM_R_MR,
            AM_R_MHLI,
            AM_R_MHLD,
            AM_MHLI_R,
            AM_MHLD_R,
            AM_R_A8,
            AM_A8_R,
            AM_MHL_SPR,
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
            u8 cycles;
            instruction_type type;
            addr_mode mode;
            reg_type reg_1;
            reg_type reg_2;
            cond_type cond;
            u8 extra_cycles;
            u8 param;
        };
        
        using a = SharpSM83;
        const instruction lookup[0x100] = {
            {4, IT_NOP, AM_IMP},
            {12, IT_LD, AM_R_D16, RT_BC},
            {8, IT_LD, AM_MR_R, RT_BC, RT_A},
            {8, IT_INC, AM_R, RT_BC},
            {4, IT_INC, AM_R, RT_B},
            {4, IT_DEC, AM_R, RT_B},
            {8, IT_LD, AM_R_D8, RT_B},
            {4, IT_RLCA},
            {20, IT_LD, AM_A16_R, RT_NONE, RT_SP},
            {8, IT_ADD, AM_R_R, RT_HL, RT_BC},
            {8, IT_LD, AM_R_MR, RT_A, RT_BC},
            {8, IT_DEC, AM_R, RT_BC},
            {4, IT_INC, AM_R, RT_C},
            {4, IT_DEC, AM_R, RT_C},
            {8, IT_LD, AM_R_D8, RT_C},
            {4, IT_RRCA},
            
            {4, IT_STOP},
            {12, IT_LD, AM_R_D16, RT_DE},
            {8, IT_LD, AM_MR_R, RT_DE, RT_A},
            {8, IT_INC, AM_R, RT_DE},
            {4, IT_INC, AM_R, RT_D},
            {4, IT_DEC, AM_R, RT_D},
            {8, IT_LD, AM_R_D8, RT_D},
            {4, IT_RLA},
            {12, IT_JR, AM_D8},
            {8, IT_ADD, AM_R_R, RT_HL, RT_DE},
            {8, IT_LD, AM_R_MR, RT_A, RT_DE},
            {8, IT_DEC, AM_R, RT_DE},
            {4, IT_INC, AM_R, RT_E},
            {4, IT_DEC, AM_R, RT_E},
            {8, IT_LD, AM_R_D8, RT_E},
            {4, IT_RRA},

            {8, IT_JR, AM_D8, RT_NONE, RT_NONE, CT_NZ, 4},
            {12, IT_LD, AM_R_D16, RT_HL},
            {8, IT_LD, AM_MHLI_R, RT_HL, RT_A},
            {8, IT_INC, AM_R, RT_HL},
            {4, IT_INC, AM_R, RT_H},
            {4, IT_DEC, AM_R, RT_H},
            {8, IT_LD, AM_R_D8, RT_H},
            {4, IT_DAA},
            {8, IT_JR, AM_D8, RT_NONE, RT_NONE, CT_Z, 4},
            {8, IT_ADD, AM_R_R, RT_HL, RT_HL},
            {8, IT_LD, AM_R_MHLI, RT_A, RT_HL},
            {8, IT_DEC, AM_R, RT_HL},
            {4, IT_INC, AM_R, RT_L},
            {4, IT_DEC, AM_R, RT_L},
            {8, IT_LD, AM_R_D8, RT_L},
            {4, IT_CPL},

            {8, IT_JR, AM_D8, RT_NONE, RT_NONE, CT_NC, 4},
            {12, IT_LD, AM_R_D16, RT_SP},
            {8, IT_LD, AM_MHLD_R, RT_HL, RT_A},
            {8, IT_INC, AM_R, RT_SP},
            {12, IT_INC, AM_MR, RT_HL},
            {12, IT_DEC, AM_MR, RT_HL},
            {12, IT_LD, AM_MR_D8, RT_HL},
            {4, IT_SCF},
            {8, IT_JR, AM_D8, RT_NONE, RT_NONE, CT_C, 4},
            {8, IT_ADD, AM_R_R, RT_HL, RT_SP},
            {8, IT_LD, AM_R_MHLD, RT_A, RT_HL},
            {8, IT_DEC, AM_R, RT_SP},
            {4, IT_INC, AM_R, RT_A},
            {4, IT_DEC, AM_R, RT_A},
            {8, IT_LD, AM_R_D8, RT_A},
            {4, IT_CCF},

            {4, IT_LD, AM_R_R, RT_B, RT_B},
            {4, IT_LD, AM_R_R, RT_B, RT_C},
            {4, IT_LD, AM_R_R, RT_B, RT_D},
            {4, IT_LD, AM_R_R, RT_B, RT_E},
            {4, IT_LD, AM_R_R, RT_B, RT_H},
            {4, IT_LD, AM_R_R, RT_B, RT_L},
            {8, IT_LD, AM_R_MR, RT_B, RT_HL},
            {4, IT_LD, AM_R_R, RT_B, RT_A},
            {4, IT_LD, AM_R_R, RT_C, RT_B},
            {4, IT_LD, AM_R_R, RT_C, RT_C},
            {4, IT_LD, AM_R_R, RT_C, RT_D},
            {4, IT_LD, AM_R_R, RT_C, RT_E},
            {4, IT_LD, AM_R_R, RT_C, RT_H},
            {4, IT_LD, AM_R_R, RT_C, RT_L},
            {8, IT_LD, AM_R_MR, RT_C, RT_HL},
            {4, IT_LD, AM_R_R, RT_C, RT_A},

            {4, IT_LD, AM_R_R,  RT_D, RT_B},
            {4, IT_LD, AM_R_R,  RT_D, RT_C},
            {4, IT_LD, AM_R_R,  RT_D, RT_D},
            {4, IT_LD, AM_R_R,  RT_D, RT_E},
            {4, IT_LD, AM_R_R,  RT_D, RT_H},
            {4, IT_LD, AM_R_R,  RT_D, RT_L},
            {8, IT_LD, AM_R_MR, RT_D, RT_HL},
            {4, IT_LD, AM_R_R,  RT_D, RT_A},
            {4, IT_LD, AM_R_R,  RT_E, RT_B},
            {4, IT_LD, AM_R_R,  RT_E, RT_C},
            {4, IT_LD, AM_R_R,  RT_E, RT_D},
            {4, IT_LD, AM_R_R,  RT_E, RT_E},
            {4, IT_LD, AM_R_R,  RT_E, RT_H},
            {4, IT_LD, AM_R_R,  RT_E, RT_L},
            {8, IT_LD, AM_R_MR, RT_E, RT_HL},
            {4, IT_LD, AM_R_R,  RT_E, RT_A},
            
            {4, IT_LD, AM_R_R,  RT_H, RT_B},
            {4, IT_LD, AM_R_R,  RT_H, RT_C},
            {4, IT_LD, AM_R_R,  RT_H, RT_D},
            {4, IT_LD, AM_R_R,  RT_H, RT_E},
            {4, IT_LD, AM_R_R,  RT_H, RT_H},
            {4, IT_LD, AM_R_R,  RT_H, RT_L},
            {8, IT_LD, AM_R_MR, RT_H, RT_HL},
            {4, IT_LD, AM_R_R,  RT_H, RT_A},
            {4, IT_LD, AM_R_R,  RT_L, RT_B},
            {4, IT_LD, AM_R_R,  RT_L, RT_C},
            {4, IT_LD, AM_R_R,  RT_L, RT_D},
            {4, IT_LD, AM_R_R,  RT_L, RT_E},
            {4, IT_LD, AM_R_R,  RT_L, RT_H},
            {4, IT_LD, AM_R_R,  RT_L, RT_L},
            {8, IT_LD, AM_R_MR, RT_L, RT_HL},
            {4, IT_LD, AM_R_R,  RT_L, RT_A},

            {8, IT_LD, AM_MR_R,  RT_HL, RT_B},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_C},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_D},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_E},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_H},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_L},
            {4, IT_HALT},
            {8, IT_LD, AM_MR_R,  RT_HL, RT_A},
            {4, IT_LD, AM_R_R,  RT_A, RT_B},
            {4, IT_LD, AM_R_R,  RT_A, RT_C},
            {4, IT_LD, AM_R_R,  RT_A, RT_D},
            {4, IT_LD, AM_R_R,  RT_A, RT_E},
            {4, IT_LD, AM_R_R,  RT_A, RT_H},
            {4, IT_LD, AM_R_R,  RT_A, RT_L},
            {8, IT_LD, AM_R_MR, RT_A, RT_HL},
            {4, IT_LD, AM_R_R,  RT_A, RT_A},
            
            {4, IT_ADD, AM_R_R, RT_A, RT_B},
            {4, IT_ADD, AM_R_R, RT_A, RT_C},
            {4, IT_ADD, AM_R_R, RT_A, RT_D},
            {4, IT_ADD, AM_R_R, RT_A, RT_E},
            {4, IT_ADD, AM_R_R, RT_A, RT_H},
            {4, IT_ADD, AM_R_R, RT_A, RT_L},
            {8, IT_ADD, AM_R_MR, RT_A, RT_HL},
            {4, IT_ADD, AM_R_R, RT_A, RT_A},
            {4, IT_ADC, AM_R_R, RT_A, RT_B},
            {4, IT_ADC, AM_R_R, RT_A, RT_C},
            {4, IT_ADC, AM_R_R, RT_A, RT_D},
            {4, IT_ADC, AM_R_R, RT_A, RT_E},
            {4, IT_ADC, AM_R_R, RT_A, RT_H},
            {4, IT_ADC, AM_R_R, RT_A, RT_L},
            {8, IT_ADC, AM_R_MR, RT_A, RT_HL},
            {4, IT_ADC, AM_R_R, RT_A, RT_A},

            {4, IT_SUB, AM_R_R, RT_A, RT_B},
            {4, IT_SUB, AM_R_R, RT_A, RT_C},
            {4, IT_SUB, AM_R_R, RT_A, RT_D},
            {4, IT_SUB, AM_R_R, RT_A, RT_E},
            {4, IT_SUB, AM_R_R, RT_A, RT_H},
            {4, IT_SUB, AM_R_R, RT_A, RT_L},
            {8, IT_SUB, AM_R_MR, RT_A, RT_HL},
            {4, IT_SUB, AM_R_R, RT_A, RT_A},
            {4, IT_SBC, AM_R_R, RT_A, RT_B},
            {4, IT_SBC, AM_R_R, RT_A, RT_C},
            {4, IT_SBC, AM_R_R, RT_A, RT_D},
            {4, IT_SBC, AM_R_R, RT_A, RT_E},
            {4, IT_SBC, AM_R_R, RT_A, RT_H},
            {4, IT_SBC, AM_R_R, RT_A, RT_L},
            {8, IT_SBC, AM_R_MR, RT_A, RT_HL},
            {4, IT_SBC, AM_R_R, RT_A, RT_A},

            {4, IT_AND, AM_R_R, RT_A, RT_B},
            {4, IT_AND, AM_R_R, RT_A, RT_C},
            {4, IT_AND, AM_R_R, RT_A, RT_D},
            {4, IT_AND, AM_R_R, RT_A, RT_E},
            {4, IT_AND, AM_R_R, RT_A, RT_H},
            {4, IT_AND, AM_R_R, RT_A, RT_L},
            {8, IT_AND, AM_R_MR, RT_A, RT_HL},
            {4, IT_AND, AM_R_R, RT_A, RT_A},
            {4, IT_XOR, AM_R_R, RT_A, RT_B},
            {4, IT_XOR, AM_R_R, RT_A, RT_C},
            {4, IT_XOR, AM_R_R, RT_A, RT_D},
            {4, IT_XOR, AM_R_R, RT_A, RT_E},
            {4, IT_XOR, AM_R_R, RT_A, RT_H},
            {4, IT_XOR, AM_R_R, RT_A, RT_L},
            {8, IT_XOR, AM_R_MR, RT_A, RT_HL},
            {4, IT_XOR, AM_R_R, RT_A, RT_A},

            {4, IT_OR, AM_R_R, RT_A, RT_B},
            {4, IT_OR, AM_R_R, RT_A, RT_C},
            {4, IT_OR, AM_R_R, RT_A, RT_D},
            {4, IT_OR, AM_R_R, RT_A, RT_E},
            {4, IT_OR, AM_R_R, RT_A, RT_H},
            {4, IT_OR, AM_R_R, RT_A, RT_L},
            {8, IT_OR, AM_R_MR, RT_A, RT_HL},
            {4, IT_OR, AM_R_R, RT_A, RT_A},
            {4, IT_CP, AM_R_R, RT_A, RT_B},
            {4, IT_CP, AM_R_R, RT_A, RT_C},
            {4, IT_CP, AM_R_R, RT_A, RT_D},
            {4, IT_CP, AM_R_R, RT_A, RT_E},
            {4, IT_CP, AM_R_R, RT_A, RT_H},
            {4, IT_CP, AM_R_R, RT_A, RT_L},
            {8, IT_CP, AM_R_MR, RT_A, RT_HL},
            {4, IT_CP, AM_R_R, RT_A, RT_A},

            {8, IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_NZ, 12},
            {12, IT_POP, AM_R, RT_BC},
            {12, IT_JP, AM_D16, RT_NONE, RT_NONE, CT_NZ, 4},
            {16, IT_JP, AM_D16},
            {12, IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_NZ, 12},
            {16, IT_PUSH, AM_R, RT_BC},
            {8, IT_ADD, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x00},
            {8, IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_Z, 12},
            {16, IT_RET},
            {12, IT_JP, AM_D16, RT_NONE, RT_NONE, CT_Z, 4},
            {4, IT_CB, AM_D8},
            {12, IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_Z, 12},
            {24, IT_CALL, AM_D16},
            {8, IT_ADC, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x08},

            {8, IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_NC, 12},
            {12, IT_POP, AM_R, RT_DE},
            {12, IT_JP, AM_D16, RT_NONE, RT_NONE, CT_NC, 4},
            {0, IT_NONE},
            {12, IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_NC, 12},
            {16, IT_PUSH, AM_R, RT_DE},
            {8, IT_SUB, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x10},
            {8, IT_RET, AM_IMP, RT_NONE, RT_NONE, CT_C, 12},
            {16, IT_RETI},
            {12, IT_JP, AM_D16, RT_NONE, RT_NONE, CT_C, 4},
            {0, IT_NONE},
            {12, IT_CALL, AM_D16, RT_NONE, RT_NONE, CT_C, 12},
            {0, IT_NONE},
            {8, IT_SBC, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x18},
            
            {12, IT_LDH, AM_A8_R, RT_NONE, RT_A},
            {12, IT_POP, AM_R, RT_HL},
            {8, IT_LD, AM_MR_R, RT_C, RT_A},
            {0, IT_NONE},
            {0, IT_NONE},
            {16, IT_PUSH, AM_R, RT_HL},
            {8, IT_AND, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x20},
            {16, IT_ADD, AM_R_D8, RT_SP},
            {4, IT_JP, AM_R, RT_HL},
            {16, IT_LD, AM_A16_R, RT_NONE, RT_A},
            {0, IT_NONE},
            {0, IT_NONE},
            {0, IT_NONE},
            {8, IT_XOR, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x28},
            
            {12, IT_LDH, AM_R_A8, RT_A},
            {12, IT_POP, AM_R, RT_AF},
            {8, IT_LD, AM_R_MR, RT_A, RT_C},
            {4, IT_DI},
            {0, IT_NONE},
            {16, IT_PUSH, AM_R, RT_AF},
            {8, IT_OR, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x30},
            {12, IT_LD, AM_MHL_SPR, RT_HL, RT_SP},
            {8, IT_LD, AM_R_R, RT_SP, RT_HL},
            {16, IT_LD, AM_R_A16, RT_A},
            {4, IT_EI},
            {0, IT_NONE},
            {0, IT_NONE},
            {8, IT_CP, AM_R_D8, RT_A},
            {16, IT_RST, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0, 0x38},
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
        fetch_info_t fetch_info = {0, {0, IT_NONE}, false, 0, 0};

        u16 regs[6];

        Bus *bus;
        bool running;

        const instruction get_instruction(u8 opcode);
        const char *instruction_name(instruction_type t);

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
