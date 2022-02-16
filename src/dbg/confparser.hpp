#pragma once

#ifndef DEBUG
#define DEBUG
#endif

#include <string>
#include <vector>
#include "../gbc/cpu.hpp"
#include "../gbc/gameboy.hpp"

class Rule{
    public:
        enum operand{
            EQ, GT, LS, LE, GE, NE, NONE,
        };
        enum val_type{
            REG,
            MREG,
            NUM,
            ADDR
        };
    private:
        operand op;
        u32 val_a, val_b;
        val_type ta, tb;
        bool compare(u16 a, u16 b);
        u16 get_val(GameBoy &gb, u32 val, val_type t);
        
    public:
        Rule(u32 a, val_type ta, operand op, u32 b, val_type tb);
        ~Rule();
        bool check(GameBoy &gb);
};


class ConfParser{
    private:
        std::vector<Rule> rules;
        std::pair<u32, Rule::val_type> parse_token(std::string, size_t line);
    public:
        ConfParser();
        ~ConfParser();

        void parse(const char *);
        bool check(GameBoy &gb);
};
