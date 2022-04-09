#include "lcd.hpp"
#include "../../common/assert.hpp"

LCD::LCD(int pixel_size){
    this->window_width = DISPLAY_WIDTH*pixel_size;
    this->window_height = DISPLAY_HEIGHT*pixel_size;

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

void LCD::clear(){
    static const u32 val = 0x000000FF;
    for (u32 i = 0; i < DISPLAY_HEIGHT; i++)
        for (u32 j = 0; j < DISPLAY_WIDTH; j++)
            this->buffer[i][j] = val;
}

void LCD::draw_pixel(u16 x, u16 y){
    SDL_Rect rect = (SDL_Rect){.x = x*pixel_size, .y = y*pixel_size, .w =pixel_size, .h=pixel_size};

    u32 rgba = buffer[y][x];
    u8 red = (rgba >> 3*8) & 0xFF;
    u8 green = (rgba >> 2*8) & 0xFF;
    u8 blue = (rgba >> 1*8) & 0xFF;
    u8 alpha = rgba & 0xFF;

    SDL_SetRenderDrawColor(ren, red, green, blue, alpha);
    ASSERT(SDL_RenderFillRect(ren, &rect) == 0, SDL_GetError());
}

void LCD::draw(){
    for (u32 i = 0; i < DISPLAY_HEIGHT; i++)
        for (u32 j = 0; j < DISPLAY_WIDTH; j++)
            draw_pixel(j, i);

    SDL_RenderPresent(ren);
}
