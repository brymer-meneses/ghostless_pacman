// Standard Library Imports
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

// Project-wide Headers
#include "utils.h"
#include "logic.h"
#include "state.h"
#include "assets.h"
#include "declarations.h"


int main (int argc, char *argv[]) {

    init_SDL();

    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    // Set the background color
    SDL_SetRenderDrawColor(renderer, 24, 28, 36, 1);
     
    // Load all game assets
    Assets *assets = load_all_assets(renderer);

    // Initialize map which will be used in the game
    Map *map = init_map(assets, 2);
     
    States states = {
        // Set the initial state of the player
        .player_state = PLAYER_IN_MENU, 

        // Set the initial state of the game when th player enters th game from 
        // the menu.
        .game_state = GAME_IN_FOOD_NUMBER_INPUT,

        // Set the initial index for the tutorial slides 
        .current_tutorial_slide_index = 0,

        // Set the initial wrong key state
        .wrong_key_state = WRONG_KEY_NONE,

        // Set the initial number of foods. This value will change later on.
        .current_number_of_foods_picked = 1,
    };

    // Play background music
    Mix_FadeInMusic(assets->sounds.background_music, -1, 4000);

    bool user_wants_to_quit = false;
    while (!user_wants_to_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                user_wants_to_quit = true; 
            else if (event.type == SDL_KEYDOWN) 
                handle_keypress(event, &states, map, assets);
        }
        SDL_RenderClear(renderer);
    
        handle_state(&states, map, assets);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60); 
    }

    free_all_assets(assets);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
