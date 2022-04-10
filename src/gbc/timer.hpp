#pragma once
#include "../common/defs.hpp"
#include "cpu.hpp"

class SharpSM83;

class Timer {
    public:
        SharpSM83 *cpu;
        u16 div = 0;
        u8 tima = 0;
        u8 tma = 0;
        u8 tac = 0;

        Timer();
        ~Timer();

        void tick();
        void write(u16, u8);
        u8 read(u16);
        void connect(SharpSM83 *);
};
