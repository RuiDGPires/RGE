#pragma once
#include "../../common/defs.hpp"

class Joypad {
    public:
        bool act_start = false;
        bool act_select = false;
        bool act_a = false;
        bool act_b = false;
        bool dir_up = false;
        bool dir_down = false;
        bool dir_left = false;
        bool dir_right = false;

        bool action_selected = false;
        bool direction_selected = false;

        u8 read();
        void select(u8);

        Joypad();
        ~Joypad();
};
