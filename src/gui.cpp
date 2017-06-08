// Copyrgith Ennis Massey 27/04/17
//
// Created by Ennis Massey on 27/04/17.
//

#include "gui.h"

template<typename Type, int Size>
void printarray(Type const(&array)[Size]) {
    for (int i = 0; i < Size; i++)
        std::cout << array[i] << std::endl;
}


int8_t render() {
    SDL_Log("We are booting");
    int8_t boot_status = boot_sdl(64, 32, (char *) "Test");
    switch (boot_status) {
        case -1:
            printf("Failed to boot SDL, error was in SDL_INIT_VIDEO\n");
            return -1;
        case -2:
            printf("Failed to boot SDL, error was in SDL_INIT_AUDIO\n");
            return -2;
        case -3:
            printf("Failed to boot SDL, error was in SDL_INIT_EVENTS\n");
            return -3;
        default:
            break;
    }
    SDL_Event event;
    for (;;) {
        // Clear screen, otherwise we get flickering and other crud
        SDL_RenderClear(windowRenderer);

        // Update window size struct
        SDL_GetWindowSize(window, &windowState.width, &windowState.height);

        // Double cast to reduce accuracy
        windowState.scaleX = (float) (double) windowState.width / 64;
        windowState.scaleY = (float) (double) windowState.height / 32;

        // Update the scaling to size of window
        // TODO: Optimise!!
        SDL_RenderSetScale(windowRenderer, windowState.scaleX, windowState
                .scaleY);
        SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG,
                "Before loop- Width: %d, Height: %d, ScaleX: %f, ScaleY: %f",
                windowState.width,
                windowState.height,
                windowState.scaleX,
                windowState.scaleY
        );
        // Poll for QUIT event
        SDL_PollEvent(&event);

        // TODO: Replace with call to render()
        for (int index = 0; index < 10; index++) {
            for (int index2 = 0; index2 < 20; index2++) {
                SDL_Rect tempRect;
                tempRect.x = index;
                tempRect.y = index2;
                tempRect.h = 1;
                tempRect.w = 1;
                SDL_RenderFillRect(windowRenderer, &tempRect);
            }
        }

        // Render to screen
        SDL_RenderPresent(windowRenderer);

        // Quit if needed
        if (event.type == SDL_QUIT) {
            SDL_Log("We are quitting m9");
            SDL_Quit();
            break;
        }
    }
}


int8_t boot_sdl(int32_t width, int32_t height, char *name) {
    // Initialise subsystems
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Error occured in boot_sdl(), Error was: %s\n",
                     SDL_GetError());
        return -1;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Error occured in boot_sdl(), Error was: %s\n",
                     SDL_GetError());
        return -2;
    }

    if (SDL_Init(SDL_INIT_EVENTS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Error occured in boot_sdl(), Error was: %s\n",
                     SDL_GetError());
        return -3;
    }

    // Create window
    window = SDL_CreateWindow(name,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              SDL_WINDOW_RESIZABLE
    );
    if (window == NULL) {
        std::cout << SDL_GetError() << std::endl;
    }

    // Create renderer
    windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (windowRenderer == NULL) {
        std::cout << SDL_GetError() << std::endl;
    }
    // Create surface for intermediate rendering
    screenSurface = SDL_CreateRGBSurface(
            0,
            width,
            height,
            32,
            0,
            0,
            0,
            0x00
    );

    // Scale Renderer
    SDL_RenderSetScale(windowRenderer, (float) 800 / 64, (float) 640 / 32);
    SDL_RenderPresent(windowRenderer);

    // Scale Window
    SDL_SetWindowSize(window, 800, 600);

    // Set WindowData struct
    windowState.height = height;
    windowState.width = width;
    windowState.scaleX = (float) width / 64;
    windowState.scaleY = (float) height / 32;
    if (screenSurface == NULL) {
        std::cout << SDL_GetError() << std::endl;
    }
    return 0;
}