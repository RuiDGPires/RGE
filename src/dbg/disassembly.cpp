#include "disassembly.hpp"
#include <string.h>

std::string decode_reg(SharpSM83::reg_type reg){
    if (reg == SharpSM83::reg_type::RT_NONE)
        return reg_str[0];

    u8 order = (reg >> 16);

    std::string str = "";

    if (reg & 0xFF00)
        str += reg_str[order*2 + 1];
    if (reg & 0x00FF)
        str += reg_str[order*2 + 2];

    return str;
}

std::string envolve(std::string str){
    return "(" + str + ")";
}
std::string hex(u32 val, bool prefix, u32 n){
    char tmp[10];
    sprintf (tmp, "%x", val);
    u8 rest = strlen(tmp) % n != 0? n - (strlen (tmp) % n): 0;
    std::string str = (prefix? std::string ("0x"): "") + std::string (rest, '0');
    for (size_t i = 0; i < strlen (tmp); i++){
        if ((i + rest) % n == 0 && i != 0)
            str += " ";
        str += tmp[i];
    }
    return str;
}  

SharpSM83::reg_type parse_reg(std::string token){
    for (size_t i= 0; i < token.size(); i++)
        token[i] = toupper(token[i]);

    if (token == "A")
        return SharpSM83::RT_A;
    else if (token == "F")
        return SharpSM83::RT_F;
    else if (token == "B")
        return SharpSM83::RT_B;
    else if (token == "C")
        return SharpSM83::RT_C;
    else if (token == "D")
        return SharpSM83::RT_D;
    else if (token == "E")
        return SharpSM83::RT_E;
    else if (token == "H")
        return SharpSM83::RT_H;
    else if (token == "L")
        return SharpSM83::RT_L;
    else if (token == "AF")
        return SharpSM83::RT_AF;
    else if (token == "BC")
        return SharpSM83::RT_BC;
    else if (token == "DE")
        return SharpSM83::RT_BC;
    else if (token == "HL")
        return SharpSM83::RT_HL;
    else if (token == "SP")
        return SharpSM83::RT_SP;
    else if (token == "PC")
        return SharpSM83::RT_PC;

    return SharpSM83::RT_NONE;
}

bool is_hex(std::string s){
    if (s[0] != '0') return false;
    if (s[1] != 'x') return false;
    s.erase(0,2);

    for (char c : s)
        if (!((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))) return false;

    return true;
}

bool is_int(std::string s){
    for (char c : s)
        if (!(c >= '0' && c <= '9')) return false;

    return true;
}
