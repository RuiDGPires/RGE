#include "input.hpp"
#include "../common/defs.hpp"
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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

static char get_esc(){
    int flags = fcntl(STDIN_FILENO, F_GETFL);

    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    char c = 0;
    c = getchar();

    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);

    if (c == (char) EOF)
        c = 0;
    return c;
}

Key get_key(bool non_block){
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    
    if (non_block)
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    char c = 0;

    c = getchar();
    

    if (c == '\n')
        return K_ENTER;

    if (c >= ' ' && c <= '~')
        return (Key)(c - ' ' + K_SPACE);

    if (c >= ('A' & 0x1F) && c <= ('Z' & 0x1F))
        return (Key) (c - ('A' & 0x1F) + (u32) K_CTRL_A);

    if (c == 127)
        return K_BCKSPACE;

    if (c == '\033'){
        c = get_esc();
        if (c == 0)
            return K_ESC;
        else if (c == 91){
            c = getchar();
            if (c == 51)
                if (getchar() == 126)
                    return K_DEL;
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

    if (non_block)
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    return K_NONE;
}

char key_to_ascii(Key key){
    if (key >= K_SPACE && key <= K_TIL)
        return (char) (key - K_SPACE) + ' ';
    else return 0;
}

void wait_input(){
    Key key = get_key();
    if (key != K_NONE) execute(key);
}

void bind_key(Key key, void (*f)(void)){
    key_map[key] = f;    
}
