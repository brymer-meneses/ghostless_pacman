#include "utils.h"
#include "declarations.h"
#include "logic.h"
#include "assets.h"


Assets *load_all_assets(SDL_Renderer *renderer, Map *map) { 


    Assets *assets = malloc(1 * sizeof(Assets));
    if (!assets) {
        puts("Memory Allocation for Assets failed");
    }


    // Load all sprites used for the game

    SDL_Rect pacman_rect = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "../assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {.x=GRID_POSITION_X, .y=GRID_POSITION_Y, .h=GRID_SIZE, .w=GRID_SIZE};
    Sprite grid = load_sprite(renderer, "../assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {.x=GRID_POSITION_X, .y=GRID_POSITION_Y, .h=33, .w=23};
    Sprite exit = load_sprite(renderer, "../assets/exit.png", 0, 1, exit_rect);


    SDL_Rect block_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=27, .w=23};


    assets->game.pacman = pacman;
    assets->game.grid = grid;
    assets->game.exit = exit;

    assets->game.blocks = (Sprite*) malloc(map->number_of_blocks * sizeof(Sprite));
    assets->game.foods = (Sprite*) malloc(map->number_of_foods * sizeof(Sprite));

    for (int i=0; i<map->number_of_blocks; i++) {
        assets->game.blocks[i] = load_sprite(renderer, "../assets/box.png", 0 , 1, block_rect);
    }

    for (int i=0; i<map->number_of_foods; i++) {
        assets->game.foods[i] = load_sprite(renderer, "../assets/food.png", 0 , 1, food_rect);
    }


    SDL_Rect homescreen_rect = {0, 0, 640, 640};
    Sprite homescreen = load_sprite(renderer, "../assets/homescreen.png", 0, 1, homescreen_rect);

    assets->misc.homescreen = homescreen;

    return assets;
}


