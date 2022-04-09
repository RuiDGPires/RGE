#include "classes.hpp"
#include "../mem_map.hpp"

Mapper::Mapper(){}
Mapper::~Mapper(){}

MCB0::MCB0(){}
MCB0::~MCB0(){}
bool MCB0::map_read(u16 addr, u32 *new_addr, bool *rom){
    if (addr >= ROM0_BEGIN && addr <= ROM1_END){
        *rom = true;
        *new_addr = addr - ROM0_BEGIN;
        return true;
    }
    return false;
}
bool MCB0::map_write(u16 addr, u32 *new_addr, u8 data, bool *rom){
    if (addr >= ROM0_BEGIN && addr <= ROM1_END){
        *rom = true;
        *new_addr = addr - ROM0_BEGIN;
        return true;
    }
    return false;
}

MCB1::MCB1(){}
MCB1::~MCB1(){}
bool MCB1::map_read(u16 addr, u32 *new_addr, bool *rom){
    if (addr >= ROM0_BEGIN && addr <= ROM1_END){
        *rom = true;
        *new_addr = addr - ROM0_BEGIN;
        return true;
    }
    return false;
}

bool MCB1::map_write(u16 addr, u32 *new_addr, u8 data, bool *rom){
    if (addr <= 0x1FFF)
        this->ram_enabled = (data & 0x0F) == 0xA;

    if (addr >= ROM0_BEGIN && addr <= ROM1_END){
        *rom = true;
        return true;
    }

    if ((addr & 0xE000) == 0x2000) {
        //rom bank number
        if (data == 0) {
            data = 1;
        }

        data &= 0b11111;

        this->rom_bank_val = data;
        this->rom_bank = 0x4000 * rom_bank_val;
    }

    if ((addr & 0xE000) == 0x4000) {
        //ram bank number
        ram_bank_val = data & 0b11;

        if (ram_enabled) {
            //if (cart_need_save()) {
            //    cart_battery_save();
            //}

            ram_bank = ram_bank_val*0x2000;
        }
    }

    if ((addr & 0xE000) == 0x6000) {
        //banking mode select
        banking_mode = data & 1;

        if (banking_mode) {
            //if (cart_need_save()) {
            //    cart_battery_save();
            //}

            ram_bank = ram_bank_val * 0x2000;
        }
    }

    if ((addr & 0xE000) == 0xA000) {
        if (!ram_enabled) {
            return true; // TODO return error
        }

        if (!ram_bank) {
            return true; // TODO return error
        }

        *new_addr = (u32)(addr - 0xA000)*0x2000;

        //if (ctx.battery) {
        //    ctx.need_save = true;
        //}
    }

    return false;
}

MCB2::MCB2(){}
MCB2::~MCB2(){}
bool MCB2::map_read(u16 addr, u32 *new_addr, bool *rom){
    return 0;
}
bool MCB2::map_write(u16 addr, u32 *new_addr, u8 data, bool *rom){
    return 0;
}

MCB3::MCB3(){}
MCB3::~MCB3(){}
bool MCB3::map_read(u16 addr, u32 *new_addr, bool *rom){
    return 0;
}
bool MCB3::map_write(u16 addr, u32 *new_addr, u8 data, bool *rom){
    return 0;
}

MCB30::MCB30(){}
MCB30::~MCB30(){}
bool MCB30::map_read(u16 addr, u32 *new_addr, bool *rom){
    return 0;
}
bool MCB30::map_write(u16 addr, u32 *new_addr, u8 data, bool *rom){
    return 0;
}
