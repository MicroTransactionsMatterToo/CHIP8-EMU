// Copyrgith Ennis Massey 27/04/17
//
// Created by Ennis Massey on 27/04/17.
//

#ifndef CHIP8_EMU_GUI_H
#define CHIP8_EMU_GUI_H

#include "CPU.h"

#include "SDL.h"
#include "SDL_log.h"

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Renderer* windowRenderer = NULL;

typedef struct WindowState_ {
    int32_t height;
    int32_t width;
    float scaleX;
    float scaleY;
} WindowState;


WindowState windowState;

int8_t boot_sdl(int32_t width, int32_t height, char *name);
int8_t render();

#endif //CHIP8_EMU_GUI_H
