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
         * \return true if condition is met, false otherwise
         */
        virtual bool check(GameBoy &gb) = 0;

        /**
         * Get string representation of breakpoint 
         */
        virtual std::string str() = 0;
};

/*********************************************//**
 * \brief Breakpoint with a single condition
 ************************************************/
class SimpleBreakpoint : public Breakpoint{
    private:
        /**
         * \brief Operand of comparison between values
         */
        operand op;
        /**
         * val_a and val_b are only representative values, to be translated using this function
         * \brief Two values for comparison
         * \see get_val
         * \see ta
         * \see tb
         */
        u32 val_a, val_b;
        /**
         * \brief Types of the values
         * \see val_a
         * \see val_b
         * \todo Implement in classes and use polymorphism to simplify this process
         */
        val_type ta, tb;
        /**
         * \brief Compare two values with operand
         * \see op
         */
        bool compare(u16 a, u16 b);
        /**
         * \brief Get actual value of values
         * \see val_a
         * \see val_b
         */
        u16 get_val(GameBoy &gb, u32 val, val_type t);
        
    public:
        /**
         * \brief Constructor
         */
        SimpleBreakpoint(u32 a, val_type ta, operand op, u32 b, val_type tb);
        ~SimpleBreakpoint();
        bool check(GameBoy &gb) override;
        std::string str() override;
};

/*********************************************//**
 * \brief Breakpoint with multiple conditions
 ************************************************/
class CompositeBreakpoint : public Breakpoint{
    private:
        /**
         * All conditions must be true for the CompositeBreakpoint to be true
         * \brief Breakpoint conditions
         */
        std::vector<SimpleBreakpoint> breakpoints;
    public:
        bool enabled = true;
        /**
         * If test is true, the program exits when the breakpoint occurs
         * 
         * This is used with test ROMs that end at a loop, to check if the test was passed or not
         */
        bool test = false;

        /**
         * \brief Default constructor
         */
        CompositeBreakpoint();
        /**
         * Passes if the breakpoint is test or not.
         * \brief Contructor
         */
        CompositeBreakpoint(bool test);
        ~CompositeBreakpoint();

        /**
         * \brief Add a breakpoint rule
         */
        void append(SimpleBreakpoint r);
        bool check(GameBoy &gb) override;
        std::string str() override;
};

class ConfParser{
    private:
        /**
         * \brief Breakpoint vector
         */
        std::vector<CompositeBreakpoint> breakpoints;
        /**
         * \brief Parse a token
         * \return pair of <representative val, val type>
         * \see val_type
         * \see Breakpoint
         * \see get_val
         */
        std::pair<u32, Breakpoint::val_type> parse_token(std::string);

        /**
         * Saves if a test descriptor has been read
         */
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
