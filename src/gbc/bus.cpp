#include "bus.hpp"
#include "../common/assert.hpp"
#include "../common/defs.hpp"

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
    for (u32 i = 0; i < WRAM_SIZE; i++)
        this->ram[i] = 0;
    for (u32 i = 0; i < HRAM_SIZE; i++)
        this->hram[i] = 0;
    for (u32 i = 0; i < IO_SIZE; i++)
        this->io[i] = 0;
}

Bus::~Bus(){}


void Bus::write(u16 addr, u8 data){
    // CART ROM
    if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END){ 
        this->cart->write(addr - CART_ROM_BEGIN, data); // NO EFFECT
    }

    // CART RAM
    else if (addr >= CRAM_BEGIN && addr <= CRAM_END){
        // TODO
    }

    // VRAM
    else if (addr >= VRAM_BEGIN && addr <= VRAM_END){
        ppu->vwrite(addr-VRAM_BEGIN, data);
    }

    // WRAM
    else if (addr >= WRAM_BEGIN && addr <= WRAM_END){
        ram[addr-WRAM_BEGIN] = data;
    }

    // ERAM
    else if (addr >= ECHO_RAM_BEGIN && addr <= ECHO_RAM_END){
        ram[addr-ECHO_RAM_BEGIN] = data;
    }

    // OAM
    else if (addr >= OAM_BEGIN && addr <= OAM_END){
        ppu->oamwrite(addr-OAM_BEGIN, data);
    }

    // IO
    else if (addr >= IO_BEGIN && addr <= IO_END){
        io[addr-IO_BEGIN] = data;
    }

    // HRAM
    else if (addr >= HRAM_BEGIN && addr <= HRAM_END){
        hram[addr-HRAM_BEGIN] = data;
    }

    // Interrupt byte
    else if (addr == IE_BYTE){
        // TODO
    }
}

u8 Bus::read(u16 addr){
    // CART ROM
    if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END){
        return this->cart->read(addr - CART_ROM_BEGIN);
    } 

    // CART_RAM
    else if (addr >= CRAM_BEGIN && addr <= CRAM_END){
        // TODO
    }

    // VRAM
    else if (addr >= VRAM_BEGIN && addr <= VRAM_END){
        return ppu->vread(addr-VRAM_BEGIN);
    }

    // WRAM
    else if (addr >= WRAM_BEGIN && addr <= WRAM_END){
        return ram[addr-WRAM_BEGIN];
    }

    // ERAM
    else if (addr >= ECHO_RAM_BEGIN && addr <= ECHO_RAM_END){
        return ram[addr-ECHO_RAM_BEGIN];
    }

    // OAM
    else if (addr >= OAM_BEGIN && addr <= OAM_END){
        return ppu->oamread(addr-OAM_BEGIN);
    }

    // IO
    else if (addr >= IO_BEGIN && addr <= IO_END){
        return io[addr-IO_BEGIN];
    }

    // HRAM
    else if (addr >= HRAM_BEGIN && addr <= HRAM_END){
        return hram[addr-HRAM_BEGIN];
    }

    // Interrupt byte
    else if (addr == IE_BYTE){
        // TODO
    }
    return 0;
}

void Bus::connect_cpu(SharpSM83 *cpu){
    this->cpu = cpu;
}

void Bus::connect_cart(Cartridge *cart){
    this->cart = cart;
}
