#pragma once

#include "bus.hpp"
#include "cpu.hpp"
#include "timer.hpp"
#include "io/joypad.hpp"


class GameBoy{
    const bool debug_mode = true;
    public:
        SharpSM83 cpu;
        Joypad joypad;
        Bus mem_bus;
        Timer timer;
        Cartridge *slot = NULL;

    public:
        GameBoy();
        ~GameBoy();

        void load_rom(const char *);
        void clock();
        void turn_on();
        void turn_off();

        void print_cartridge_info();
};
