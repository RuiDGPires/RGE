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
        virtual bool check(GameBoy &gb) = 0;
};

class SimpleRule : public Rule{
    private:
        operand op;
        u32 val_a, val_b;
        val_type ta, tb;
        bool compare(u16 a, u16 b);
        u16 get_val(GameBoy &gb, u32 val, val_type t);
        
    public:
        SimpleRule(u32 a, val_type ta, operand op, u32 b, val_type tb);
        ~SimpleRule();
        bool check(GameBoy &gb) override;
};

class CompositeRule : public Rule{
    private:
        std::vector<SimpleRule> rules;
    public:
        CompositeRule();
        CompositeRule(SimpleRule r);
        CompositeRule(std::vector<SimpleRule> v);
        ~CompositeRule();
        void append(SimpleRule r);
        bool check(GameBoy &gb) override;
};

class ConfParser{
    private:
        std::vector<CompositeRule> rules;
        std::pair<u32, Rule::val_type> parse_token(std::string, size_t line);
    public:
        bool clear_term = false;
        bool info = false;
        ConfParser();
        ~ConfParser();

        void parse(const char *);
        bool check(GameBoy &gb);

        void print_info();
};
