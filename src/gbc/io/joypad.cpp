#include "joypad.hpp"

Joypad::Joypad(){

}
Joypad::~Joypad(){

}

u8 Joypad::read(){
    u8 ret = 0xCF;

    if (action_selected) {
        ret &= ~((act_a != 0) << 0);
        ret &= ~((act_b != 0) << 1);
        ret &= ~((act_select != 0) << 2);
        ret &= ~((act_start != 0) << 3);
    }

    if (direction_selected) {
        ret &= ~((dir_right != 0) << 0);
        ret &= ~((dir_left != 0) << 1);
        ret &= ~((dir_up != 0) << 2);
        ret &= ~((dir_down != 0) << 3);
    }

    return ret;
}

void Joypad::select(u8 val){
    action_selected = !(val & 0x20);
    direction_selected = !(val & 0x10);
}

