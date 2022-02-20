#pragma once

#include "../../common/defs.hpp"

class Mapper{
    protected:
        bool ram_enabled = false;
        u8 banking_mode;
        u8 rom_bank_val, ram_bank_val;
        u8 rom_bank;
        u8 ram_bank;

    public:
        Mapper();
        ~Mapper();
        virtual bool map_read(u16 addr, u32 *new_addr, bool *rom) = 0;
        virtual bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) = 0;
};

class MCB0 : public Mapper {
    public:
        MCB0();
        ~MCB0();

        bool map_read(u16 addr, u32 *new_addr, bool *rom) override;
        bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) override;
};

class MCB1 : public Mapper {
    public:
        MCB1();
        ~MCB1();

        bool map_read(u16 addr, u32 *new_addr, bool *rom) override;
        bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) override;
};

class MCB2 : public Mapper {
    public:
        MCB2();
        ~MCB2();

        bool map_read(u16 addr, u32 *new_addr, bool *rom) override;
        bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) override;
};

class MCB3 : public Mapper {
    public:
        MCB3();
        ~MCB3();

        bool map_read(u16 addr, u32 *new_addr, bool *rom) override;
        bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) override;
};

class MCB30 : public Mapper {
    public:
        MCB30();
        ~MCB30();

        bool map_read(u16 addr, u32 *new_addr, bool *rom) override;
        bool map_write(u16 addr, u32 *new_addr, u8 val, bool *rom) override;
};
