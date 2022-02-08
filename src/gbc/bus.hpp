#pragma once

#include "../common/defs.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"

#define ROM_SIZE 2000000 // bytes
#define BOOT_ROM_SIZE 0x0100// bytes
#define OAM_SIZE 160 // bytes

#define CART_ROM_BEGIN 0x0000
#define CART_ROM_END 0x7FFF

#define RAM_BEGIN 0x8000
#define RAM_END 0xDFFF
#define RAM_SIZE RAM_END - RAM_BEGIN

class SharpSM83;

class Bus{
#ifndef DEBUG
    private:
#else
    public:
#endif
        SharpSM83 *cpu;
        Cartridge *cart;
        u8 ram[RAM_SIZE];

    public:
        Bus();
        ~Bus();

        void write(u16 addr, u8 data);
        u8 read(u16 addr);

        void connect_cpu(SharpSM83 *);
        void connect_cart(Cartridge *);
};
