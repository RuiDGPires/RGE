#include <iostream>
#include "cursor.hpp"
#include <string>
#include <sstream>

void cursor_up(int n){
    for (int i = 0; i < n; i++)
        std::cout << CURSOR_UP;
}

void cursor_down(int n){
    for (int i = 0; i < n; i++)
        std::cout << CURSOR_DOWN;
}
void cursor_right(int n){
    for (int i = 0; i < n; i++)
        std::cout << CURSOR_RIGHT;
}
void cursor_left(int n){
    for (int i = 0; i < n; i++)
        std::cout << CURSOR_LEFT;
}
void cursor_pos(int x, int y){
    std::cout << "\033[" << y << ';' << x << 'H';
}

std::string cursor_pos_str(int x, int y){
    std::stringstream s;
    s << "\033[" << y << ';' << x << 'H';

    return s.str();
}
