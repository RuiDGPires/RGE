#pragma once

#include "../common/defs.hpp"
#include "cartridge.hpp"
#include "cpu.hpp"

#define ROM_SIZE 2000000 // bytes
#define BOOT_ROM_SIZE 0x0100// bytes

#define CART_ROM_BEGIN 0x0000
#define CART_ROM_END 0x7FFF

// Video RAM
#define VRAM_BEGIN 0x8000
#define VRAM_END 0x9FFF

// Cartridge RAM
#define CRAM_BEGIN 0xA000
#define CRAM_END 0xBFFF

// Work RAM 
#define WRAM_BEGIN 0xC000
#define WRAM_END 0xDFFF
#define WRAM_SIZE WRAM_END - WRAM_BEGIN + 1

// Echo RAM
#define ECHO_RAM_BEGIN 0xE000
#define ECHO_RAM_END 0xFDFF

// Sprite attribute table
#define OAM_BEGIN 0xFE00
#define OAM_END 0xFE9F

// IO
#define IO_BEGIN 0xFF00
#define IO_END 0xFF7F
#define IO_SIZE IO_END - IO_BEGIN + 1

// High RAM
#define HRAM_BEGIN 0xFF80
#define HRAM_END 0xFFFE
#define HRAM_SIZE HRAM_END - HRAM_BEGIN + 1

// Interrupt
#define IE_BYTE 0xFFFF

class SharpSM83;

class Bus{
#ifndef DEBUG
    private:
#else
    public:
#endif
        SharpSM83 *cpu;
        Cartridge *cart;
        u8 ram[WRAM_SIZE];
        u8 io[IO_SIZE]; // temporary
        u8 hram[HRAM_SIZE]; // tmp

    public:
        Bus();
        ~Bus();

        void write(u16 addr, u8 data);
        u8 read(u16 addr);

        void connect_cpu(SharpSM83 *);
        void connect_cart(Cartridge *);
};
