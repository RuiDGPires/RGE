#include "bus.hpp"
#include "mem_map.hpp"
#include "../common/assert.hpp"
#include "../common/defs.hpp"

Bus::Bus(){
    for (u32 i = 0; i < WRA0_SIZE; i++)
        this->wra0[i] = 0xFF;
    for (u32 i = 0; i < WRA1_SIZE; i++)
        this->wra1[i] = 0xFF;
    for (u32 i = 0; i < HRA_SIZE; i++)
        this->hra[i] = 0;
    for (u32 i = 0; i < IO_SIZE; i++)
        this->io[i] = 0;
}

Bus::~Bus(){}


void Bus::write(u16 addr, u8 data){
    // CART ROM
    
    if (this->cart->write(addr, data))
        return;

    // VRAM
    else if (addr >= VRA_BEGIN && addr <= VRA_END){
        // TODO
    }

    // WRA0
    else if (addr >= WRA0_BEGIN && addr <= WRA0_END){
        wra0[addr-WRA0_BEGIN] = data;
    }

    // WRA1
    else if (addr >= WRA1_BEGIN && addr <= WRA1_END){
        wra1[addr-WRA1_BEGIN] = data;
    }

    // ERAM
    else if (addr >= ECH_BEGIN && addr <= ECH_END){
        u16 real_addr = addr - ECH_BEGIN;

        if (real_addr < WRA0_SIZE)
            wra0[real_addr] = data;
        else
            wra1[real_addr-WRA0_SIZE] = data;
    }

    // OAM
    else if (addr >= OAM_BEGIN && addr <= OAM_END){
        // TODO
    }

    // IO
    else if (addr >= IO_BEGIN && addr <= IO_END){
        if (addr == 0xFF00) 
            joypad->select(data);
        else if (addr >= 0xFF04 && addr <= 0xFF07)
            this->timer->write(addr, data);
        else if (addr == 0xFF0F)
            this->cpu->IF = data;
        else
            io[addr-IO_BEGIN] = data;
    }

    // HRAM
    else if (addr >= HRA_BEGIN && addr <= HRA_END){
        hra[addr-HRA_BEGIN] = data;
    }

    // Interrupt byte
    else if (addr == IE_BEGIN){
        ie = data;
    }
}

u8 Bus::read(u16 addr){
    // CART ROM
    u8 data;
    if (this->cart->read(addr, &data))
        return data;

    // VRAM
    else if (addr >= VRA_BEGIN && addr <= VRA_END){
        // TODO
    }

    // WRA0
    else if (addr >= WRA0_BEGIN && addr <= WRA0_END){
        return wra0[addr-WRA0_BEGIN];
    }

    // WRA1
    else if (addr >= WRA1_BEGIN && addr <= WRA1_END){
        return wra1[addr-WRA1_BEGIN];
    }

    // ECHO
    else if (addr >= ECH_BEGIN && addr <= ECH_END){
        u16 real_addr = addr - ECH_BEGIN;

        if (real_addr < WRA0_SIZE)
            return wra0[real_addr];
        else
            return wra1[real_addr-WRA0_SIZE];
    }

    // OAM
    else if (addr >= OAM_BEGIN && addr <= OAM_END){
        return ppu->oamread(addr-OAM_BEGIN);
    }

    // IO
    else if (addr >= IO_BEGIN && addr <= IO_END){
        if (addr == 0xFF00) 
            return this->joypad->read();
        else if (addr >= 0xFF04 && addr <= 0xFF07)
            return this->timer->read(addr);
        else if (addr == 0xFF0F)
            return this->cpu->IF;
        else
            return io[addr-IO_BEGIN];
    }

    // HRAM
    else if (addr >= HRA_BEGIN && addr <= HRA_END){
        return hra[addr-HRA_BEGIN];
    }

    // Interrupt byte
    else if (addr == IE_BEGIN){
        return ie;
    }
    return 0xFF;
}

void Bus::connect(SharpSM83 *cpu){
    this->cpu = cpu;
}

void Bus::connect(Cartridge *cart){
    this->cart = cart;
}

void Bus::connect(Joypad *joypad){
    this->joypad = joypad;
}

void Bus::connect(Timer *timer){
    this->timer = timer;
    this->timer->div = 0xABCC;
}
