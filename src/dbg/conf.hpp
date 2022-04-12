/*********************************************//**
 * \file conf.hpp
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

        /**
         * Previous value (used to check if value has changed)
         */
        u16 previous;
        bool _first = true;
        bool check_changed(u16);
        
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

class Conf{
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
        /**
         * Saves if the NOINFO setting was read
         */
        bool no_info = false;
        /**
         * \brief Constructor
         */
        Conf();
        ~Conf();

        /**
         * May add a new breakpoint
         * \brief Parse a line of configuration
         * \param line line to be parsed
         */
        void parse_line(std::string line);
        /**
         * \brief Parse configuration file
         * \param file_name configuration file path
         */
        void parse(const char *file_name);
        /**
         * \brief Check if any breakpoint was reached
         * \param gb Gameboy being emulated
         * \param test Set to true if any test breakpoint was reached. Nothing is done if passed value is NULL
         * \return return if a breakpoint was reached
         */
        bool check(GameBoy &gb, bool *test = NULL);

        /**
         * \brief Removes a breakpoint from the list
         * \param id breakpoint id
         */
        void remove_breakpoint(int id);
        /**
         * If breakpoint is already enabled, does nothing
         * \brief Enable a breakpoint
         * \param id breakpoint id
         */
        void enable_breakpoint(int id);
        /**
         * Disables a breakpoint, but doesn't remove it.
         * If breakpoint is already disabled, does nothing
         * \brief Disable a breakpoint
         * \param id breakpoint id
         */
        void disable_breakpoint(int id);

        /**
         * \brief List breakpoints
         * \return String with all breakpoints separated by '\\n' and color coded using the color_c codes
         * \see color_c
         */
        std::string list_breakpoints();
};
