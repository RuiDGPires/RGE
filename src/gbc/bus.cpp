#include "bus.hpp"
#include "assert.h"

// 0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
// 4000	7FFF	16 KiB ROM Bank 01~NN	From cartridge, switchable bank via mapper (if any)
// 8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
// A000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
// C000	CFFF	4 KiB Work RAM (WRAM)	
// D000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1~7
// E000	FDFF	Mirror of C000~DDFF (ECHO RAM)	Nintendo says use of this area is prohibited.
// FE00	FE9F	Sprite attribute table (OAM)	
// FEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited
// FF00	FF7F	I/O Registers	
// FF80	FFFE	High RAM (HRAM)	
// FFFF	FFFF	Interrupt Enable register (IE)	

Bus::Bus(){
}

Bus::~Bus(){}

#define CART_ROM_BEGIN 0x0000
#define CART_ROM_END 0x7FFF

#define RAM_BEGIN 0x8000
#define RAM_END 0xDFFF


void Bus::write(u16 addr, u8 data){
    if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END){
        this->cart->write(addr - CART_ROM_BEGIN, data); // NO EFFECT
    }else if (addr >= RAM_BEGIN && addr <= RAM_END){
        ram[addr-RAM_BEGIN] = data;
    }
}

u8 Bus::read(u16 addr){
    if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END){
        return this->cart->read(addr - CART_ROM_BEGIN);
    }else if (addr >= RAM_BEGIN && addr <= RAM_END){
        return ram[addr-RAM_BEGIN];
    }
    return 0;
}

void Bus::connect_cpu(SharpSM83 *cpu){
    this->cpu = cpu;
}

void Bus::connect_cart(Cartridge *cart){
    this->cart = cart;
}
