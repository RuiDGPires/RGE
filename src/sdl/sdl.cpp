#include "sdl.hpp"

#include <SDL2/SDL.h>

// SDL implementation

SDL::SDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        exit(0);
    }
}

SDL::~SDL(){

}

void SDL::execute(){
    SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    for (size_t i = 0; i < 990000000;i++);
}
