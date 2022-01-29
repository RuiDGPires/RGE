#pragma once

#include "bus.hpp"
#include "../common/defs.hpp"

class Bus;

class SharpSM83{
    private:
        u8 regs[8];
        u16 pc, sp;

        Bus *bus;
        bool running;

        void write(u16 addr, u8 data);
        u8 read(u16 addr);
    public:
        SharpSM83();
        ~SharpSM83();

        void connect_bus(Bus *);
        void clock();
};
