/*********************************************//**
 * \file disassembly.hpp
 * \brief Disassembling and info printing utility 
 ************************************************/
#pragma once
#ifndef DEBUG
#define DEBUG
#endif
#include "../gbc/cpu.hpp"
#include "../common/defs.hpp"

/**
 * Lookup table to turn registers to string
 * \see decode_reg
 */
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

/**
 * Gets the textual representation of a register
 * \param the register code
 * \return Textual representation of register
 * \see parse_reg
 */
std::string decode_reg(SharpSM83::reg_type reg);
/**
 * Turn a value to it's hexadecimal representatino
 * \param val The value to be converted
 * \param prefix If true, include "0x" before the hexadecimal number
 * \param n The length of groupings (to fill with 0's)
 * \return Textual representation of register
 */
std::string hex(u32 val, bool prefix = true, u32 n = 4);
/**
 * Envolve a string in parenthesis
 * str -> (str)
 * \param str The input string
 * \return "(str)"
 */
std::string envolve(std::string str);
/**
 * Get's the register code of a register name string
 * Inverse function of decode_reg
 * \see decode_reg
 * \param token the register string
 * \return register code
 */
SharpSM83::reg_type parse_reg(std::string token);

/**
 * \return if s is hexadecimal
 */
bool is_hex(std::string s);
/**
 * \return if s is a number
 * \todo implement signed numbers
 */
bool is_int(std::string s);
