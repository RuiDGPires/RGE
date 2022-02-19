#pragma once
#include "../common/defs.hpp"

class PPU{

    public:
        PPU();
        ~PPU();

        u8 vread(u16 addr);
        void vwrite(u16 addr, u8 data);
        u8 oamread(u16 addr);
        void oamwrite(u16 addr, u8 data);
};
