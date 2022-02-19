#pragma once

#include "../../common/defs.hpp"

class Mapper{
    public:
        Mapper();
        ~Mapper();
        virtual u8 read(u16 addr) = 0;
        virtual void write(u16 addr, u8 val) = 0;
};

class MCB0 : public Mapper {
    public:
        MCB0();
        ~MCB0();

        u8 read(u16 addr) override;
        void write(u16 addr, u8 val) override;
};

class MCB1 : public Mapper {
    public:
        MCB1();
        ~MCB1();

        u8 read(u16 addr) override;
        void write(u16 addr, u8 val) override;
};

class MCB2 : public Mapper {
    public:
        MCB2();
        ~MCB2();

        u8 read(u16 addr) override;
        void write(u16 addr, u8 val) override;
};

class MCB3 : public Mapper {
    public:
        MCB3();
        ~MCB3();

        u8 read(u16 addr) override;
        void write(u16 addr, u8 val) override;
};

class MCB30 : public Mapper {
    public:
        MCB30();
        ~MCB30();

        u8 read(u16 addr) override;
        void write(u16 addr, u8 val) override;
};
