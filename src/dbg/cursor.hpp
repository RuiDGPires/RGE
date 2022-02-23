#pragma once

#include <string>

#define CURSOR_UP "\033[1A"
#define CURSOR_DOWN "\033[1B"
#define CURSOR_RIGHT "\033[1C"
#define CURSOR_LEFT "\033[1D"


std::string cursor_pos_str(int x, int y);

void cursor_up(int n = 1);
void cursor_down(int n = 1);
void cursor_left(int n = 1);
void cursor_right(int n = 1);
void cursor_pos(int x, int y);
