//********************************************
// ONLY ONE SCREEN MAY BE UP AT A GIVEN TIME
//********************************************
#pragma once

#include "component.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Screen{
    private:
        std::string buffer, display;
        std::vector<Component *> components;

    public:
        int term_w, term_h;
        Screen();
        ~Screen();

        void update_term_size();

        static void sig_handler(int signum);

        void refresh();
        Screen &operator<<(std::string);
        Screen &operator<<(Component &);
};

