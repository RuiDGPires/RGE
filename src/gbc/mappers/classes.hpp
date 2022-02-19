#pragma once

#include "../../common/defs.hpp"

class Mapper{
    protected:
        bool ram_enabled = false;
    public:
        Mapper();
        ~Mapper();
        virtual u16 map_read(u16 addr) = 0;
        virtual u16 map_write(u16 addr, u8 val) = 0;
};

class MCB0 : public Mapper {
    public:
        MCB0();
        ~MCB0();

        u16 map_read(u16 addr) override;
        u16 map_write(u16 addr, u8 val) override;
};

class MCB1 : public Mapper {
    public:
        MCB1();
        ~MCB1();

        u16 map_read(u16 addr) override;
        u16 map_write(u16 addr, u8 val) override;
};

class MCB2 : public Mapper {
    public:
        MCB2();
        ~MCB2();

        u16 map_read(u16 addr) override;
        u16 map_write(u16 addr, u8 val) override;
};

class MCB3 : public Mapper {
    public:
        MCB3();
        ~MCB3();

        u16 map_read(u16 addr) override;
        u16 map_write(u16 addr, u8 val) override;
};

class MCB30 : public Mapper {
    public:
        MCB30();
        ~MCB30();

        u16 map_read(u16 addr) override;
        u16 map_write(u16 addr, u8 val) override;
};
