#pragma once

#include "mem_map.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"
#include "ppu.hpp"
#include "timer.hpp"
#include "io/joypad.hpp"
#include "../common/defs.hpp"

class SharpSM83;
class Timer;

class Bus{
    public:
        // Devices
        SharpSM83 *cpu;
        Cartridge *cart;
        PPU *ppu;
        Joypad *joypad;
        Timer *timer;

        u8 wra0[WRA0_SIZE];
        u8 wra1[WRA1_SIZE];
        u8 hra[HRA_SIZE]; // tmp
        u8 io[IO_SIZE]; // temporary

    public:
        Bus();
        ~Bus();

        void write(u16 addr, u8 data);
        u8 read(u16 addr);

        void connect(SharpSM83 *);
        void connect(Cartridge *);
        void connect(Joypad *);
        void connect(Timer *);
};
