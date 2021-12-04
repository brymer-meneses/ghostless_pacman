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
    SDL_SetRenderDrawColor(renderer, 236, 239, 244, 1);
     
    // Load all game assets
    Assets *assets = load_all_assets(renderer);

    // Initialize map which will be used in the game
    Map *map = init_map(assets);
     
    States states = {
        // Set the initial state of the player
        .player_state = PLAYER_IN_MENU, 
        // Set the initial index for the tutorial slides 
        .current_tutorial_slide_index = 0,
    };

    // Play background music
    Mix_FadeInMusic(assets->sounds.background_music, -1, 4000);

    printf("%d", states.player_state);
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
