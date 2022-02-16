#include "lcd.hpp"
#include "../common/assert.hpp"
#include "../common/defs.hpp"

LCD::LCD(int pixel_size){
    this->window_width = DISPLAY_WIDTH/pixel_size;
    this->window_height = DISPLAY_HEIGHT/pixel_size;


    this->win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->window_width, this->window_height, 0);
    ASSERT(win != NULL, SDL_GetError());

    this->ren = SDL_CreateRenderer(win, -1, 0 );
    ASSERT(ren != NULL, SDL_GetError());

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
}

LCD::LCD() : LCD(3){

}

LCD::~LCD(){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}
