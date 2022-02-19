#include "mem_map.hpp"

const char *get_mem_name(u16 addr){
    if (addr >= ROM0_BEGIN && addr <= ROM0_END)
        return "ROM0";
    else if (addr >= ROM1_BEGIN && addr <= ROM1_END)
        return "ROM1";
    else if (addr >= VRA_BEGIN && addr <= VRA_END)
        return "VRA ";
    else if (addr >= SRA_BEGIN && addr <= SRA_END)
        return "SRA ";
    else if (addr >= WRA0_BEGIN && addr <= WRA0_END)
        return "WRA0";
    else if (addr >= WRA1_BEGIN && addr <= WRA1_END)
        return "WRA1";
    else if (addr >= ECH_BEGIN && addr <= ECH_END)
        return "ECH ";
    else if (addr >= OAM_BEGIN && addr <= OAM_END)
        return "OAM ";
    else if (addr >= IO_BEGIN && addr <= IO_END)
        return "I/O ";
    else if (addr >= HRA_BEGIN && addr <= HRA_END)
        return "HRA ";
    else if (addr >= IE_BEGIN && addr <= IE_END)
        return "IE  ";
    return "----";
}
