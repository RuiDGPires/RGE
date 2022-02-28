/*********************************************//**
 * \file confparser.hpp
 * \brief Configuration parser and breakpoint checker
 ************************************************/
#pragma once

#ifndef DEBUG
#define DEBUG
#endif

#include <string>
#include <vector>
#include "../gbc/cpu.hpp"
#include "../gbc/gameboy.hpp"

/*********************************************//**
 * \brief Base class for breakpoints
 ************************************************/
class Breakpoint{
    public:
        /**
         * Operand of comparison between values
         */
        enum operand{
            EQ, GT, LS, LE, GE, NE, NONE,
        };

        /**
         * Types of values to be compared
         */
        enum val_type{
            REG,
            MREG,
            NUM,
            ADDR
        };

        /**
         * Check if breakpoint condition is met
         */
        virtual bool check(GameBoy &gb) = 0;

        /**
         * Get string representation of breakpoint 
         */
        virtual std::string str() = 0;
};

/*********************************************//**
 * \brief Base class for rules (breakpoints)
 ************************************************/
class SimpleBreakpoint : public Breakpoint{
    private:
        operand op;
        u32 val_a, val_b;
        val_type ta, tb;
        bool compare(u16 a, u16 b);
        u16 get_val(GameBoy &gb, u32 val, val_type t);
        
    public:
        SimpleBreakpoint(u32 a, val_type ta, operand op, u32 b, val_type tb);
        ~SimpleBreakpoint();
        bool check(GameBoy &gb) override;
        std::string str() override;
};

class CompositeBreakpoint : public Breakpoint{
    private:
        std::vector<SimpleBreakpoint> rules;
    public:
        bool enabled = true;
        bool test = false;
        CompositeBreakpoint();
        CompositeBreakpoint(bool test);
        CompositeBreakpoint(SimpleBreakpoint r);
        CompositeBreakpoint(std::vector<SimpleBreakpoint> v);
        ~CompositeBreakpoint();
        void append(SimpleBreakpoint r);
        bool check(GameBoy &gb) override;
        std::string str() override;
};

class ConfParser{
    private:
        std::vector<CompositeBreakpoint> rules;
        std::pair<u32, Breakpoint::val_type> parse_token(std::string);

        bool is_test = false;
    public:
        bool no_info = false;
        ConfParser();
        ~ConfParser();

        void parse_line(std::string);
        void parse(const char *);
        bool check(GameBoy &gb, bool *test = NULL);

        void remove_breakpoint(int);
        void enable_breakpoint(int);
        void disable_breakpoint(int);

        void print_info();
        std::string list_breakpoints();
};
