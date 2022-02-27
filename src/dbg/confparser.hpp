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
        virtual std::string str() = 0;
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
        std::string str() override;
};

class CompositeRule : public Rule{
    private:
        std::vector<SimpleRule> rules;
    public:
        bool enabled = true;
        bool test = false;
        CompositeRule();
        CompositeRule(bool test);
        CompositeRule(SimpleRule r);
        CompositeRule(std::vector<SimpleRule> v);
        ~CompositeRule();
        void append(SimpleRule r);
        bool check(GameBoy &gb) override;
        std::string str() override;
};

class ConfParser{
    private:
        std::vector<CompositeRule> rules;
        std::pair<u32, Rule::val_type> parse_token(std::string);

        bool is_test = false;
    public:
        bool clear_term = false;
        bool info = false;
        ConfParser();
        ~ConfParser();

        void parse_line(std::string);
        void parse(const char *);
        bool check(GameBoy &gb, bool *test = NULL);

        void remove_rule(int);
        void enable_rule(int);
        void disable_rule(int);

        void print_info();
        std::string list_rules();
};
