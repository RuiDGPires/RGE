#include "input.hpp"
#include "../common/defs.hpp"
#include <stdio.h>
#include <map>

std::map<Key, void (*)(void)> key_map;

void setup_events(){
    for (u32 i = 0; i < K_COUNT; i++)
        key_map[(Key) i] = nullptr;
}

static void execute(Key key){
    if (key_map[key] != nullptr) key_map[key]();
}

#define ARROW_UP 'A'
#define ARROW_DOWN 'B'
#define ARROW_RIGHT 'C'
#define ARROW_LEFT 'D'

static bool special(char c){
    if (c == 49)
        if (getchar() == 59)
            return true;

    return false;
}

static Key get_key(){
    char c = getchar();
    switch (c){
        case '\n':
            return K_ENTER;
        case ' ':
            return K_SPACE;
        case ',':
            return K_COMMA;
        case '.':
            return K_DOT;
        case ';':
            return K_SEMICLN;
        case ':':
            return K_CLN;
        case '&':
            return K_AND;
        case '|':
            return K_BAR;
        case '%':
            return K_PERC;
        case '#':
            return K_CARD;
        case '$':
            return K_DOLLAR;
        case '!':
            return K_EXC;
        case '?':
            return K_QUE;
    }

    if (c >= '0' && c <= '9')
        return (Key)(c - '0' + (u32) K_0);

    if (c >= 'a' && c <= 'z')
        return (Key) (c - 'a' + (u32) K_A);
    if (c >= 'A' && c <= 'Z')
        return (Key) (c - 'A' + (u32) K_SHFT_A);
    if (c >= ('A' & 0x1F) && c <= ('Z' & 0x1F))
        return (Key) (c - ('A' & 0x1F) + (u32) K_CTRL_A);

    if (c == '\033'){ // Arrows
        if (getchar() == 91){
            c = getchar();
            if (c == ARROW_UP)
                return K_ARROW_UP;
            if (c == ARROW_DOWN)
                return K_ARROW_DOWN;
            if (c == ARROW_RIGHT)
                return K_ARROW_RIGHT;
            if (c == ARROW_LEFT)
                return K_ARROW_LEFT;

            if (special(c)){
                if (getchar() == 53){
                    c = getchar();
                    if (c == ARROW_UP)
                        return K_CTRL_ARROW_UP;
                    if (c == ARROW_DOWN)
                        return K_CTRL_ARROW_DOWN;
                    if (c == ARROW_RIGHT)
                        return K_CTRL_ARROW_RIGHT;
                    if (c == ARROW_LEFT)
                        return K_CTRL_ARROW_LEFT;
                }
            }
        }
    }
    return K_NONE;
}

void wait_input(){
    Key key = get_key();
    if (key != K_NONE) execute(key);
}

void bind_key(Key key, void (*f)(void)){
    key_map[key] = f;    
}
