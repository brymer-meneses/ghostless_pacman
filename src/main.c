// Standard Library Imports
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

// Project-wide Headers
#include "utils.h"
#include "logic.h"
#include "declarations.h"

int main (int argc, char *argv[]) {
    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    IMG_Init(IMG_INIT_PNG);
    SDL_SetRenderDrawColor(renderer, 236, 239, 244, 1);

    SDL_Rect pacman_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {GRID_POSITION_X, GRID_POSITION_Y, GRID_SIZE, GRID_SIZE};
    Sprite grid = load_sprite(renderer, "assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {GRID_POSITION_X, GRID_POSITION_Y, 33, 23};
    Sprite exit = load_sprite(renderer, "assets/exit.png", 40, 10, exit_rect);

    // NOTE: This does not work, probably use malloc next time?
    // Sprite blocks[NUMBER_OF_BLOCKS];
    // for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
    //     SDL_Rect rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    //     blocks[i] = load_sprite(renderer, "assets/box.png", 0, 0, rect);
    // }

    // Sprite foods[NUMBER_OF_FOODS];
    // for (int i=0; i<NUMBER_OF_FOODS; i++) {
    //     SDL_Rect rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=27, .w=23};
    //     foods[i] = load_sprite(renderer, "assets/food.png", 0, 0, rect);
    // }

    bool user_wants_to_quit = false;
    while (!user_wants_to_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) user_wants_to_quit = true; 
            else if (event.type == SDL_KEYDOWN) handle_keypress(event, &pacman);
        }
        SDL_RenderClear(renderer);
        display_board(&pacman, &grid);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60); }

    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
