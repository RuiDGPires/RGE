#pragma once
#include <SDL2/SDL.h>

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

class LCD{
    private:
        int pixel_size;
        int window_width, window_height;

        SDL_Window *win = NULL;
        SDL_Renderer *ren = NULL;

    public:
        LCD();
        LCD(int);
        ~LCD();
};
