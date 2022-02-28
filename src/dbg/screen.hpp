/*********************************************//**
 * \file screen.hpp
 * \brief Display System
 ************************************************/
//********************************************
// ONLY ONE SCREEN MAY BE UP AT A GIVEN TIME
//********************************************
#pragma once

#include "component.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

/**
 * Class that will hold components, clear the terminal and display their contents
 */
class Screen{
    private:
        std::string buffer, display;
        std::vector<Component *> components;

    public:
        /**
         * Terminal width in columns and terminal height in rows.
         */
        int term_w, term_h;
        /**
         * Constructor
         */
        Screen();
        ~Screen();

        /**
         * Updates term_w and term_h
         * \see term_w
         * \see term_h
         */
        void update_term_size();

        /**
         * Handler for the window update signal emited by the terminal when window is resized
         */
        static void sig_handler(int signum);

        /**
         * Refresh the terminal, prints every component and it's contents
         */
        void refresh();

        /**
         * Adds a component to the display screen
         * \param component Component to be added
         * \see Component
         */
        Screen &operator<<(Component &component);
};

