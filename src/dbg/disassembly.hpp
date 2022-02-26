#pragma once
#ifndef DEBUG
#define DEBUG
#endif
#include "../gbc/cpu.hpp"
#include "../common/defs.hpp"

const std::string reg_str[13] = {
    "<NONE>",
    "A",
    "F",
    "B",
    "C",
    "D",
    "E",
    "H",
    "L",
    "S",
    "P",
    "P",
    "C"
};


std::string decode_reg(SharpSM83::reg_type reg);
std::string hex(u32 val, bool prefix = true, u32 n = 4);
std::string envolve(std::string str);
