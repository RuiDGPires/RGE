#pragma once
#include "../common/defs.hpp"
#include <SDL2/SDL.h>

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

class LCD{
    private:
        u8 pixel_size;
        u16 window_width, window_height;

        u32 buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];

        SDL_Window *win = NULL;
        SDL_Renderer *ren = NULL;

        void clear();
        void draw_pixel(u16 x, u16 y);
        void draw();
    public:
        LCD();
        LCD(int);
        ~LCD();
};
