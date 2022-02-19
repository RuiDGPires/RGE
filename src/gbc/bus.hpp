#pragma once

#include "mem_map.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"
#include "ppu.hpp"
#include "../common/defs.hpp"

class SharpSM83;

class Bus{
#ifndef DEBUG
    private:
#else
    public:
#endif
        // Devices
        SharpSM83 *cpu;
        Cartridge *cart;
        PPU *ppu;
        u8 wra0[WRA0_SIZE];
        u8 wra1[WRA1_SIZE];
        u8 hra[HRA_SIZE]; // tmp
        u8 io[IO_SIZE]; // temporary

    public:
        Bus();
        ~Bus();

        void write(u16 addr, u8 data);
        u8 read(u16 addr);

        void connect_cpu(SharpSM83 *);
        void connect_cart(Cartridge *);
};
