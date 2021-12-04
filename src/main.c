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
    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    // Set the background color
    SDL_SetRenderDrawColor(renderer, 236, 239, 244, 1);
     
    // Initialize map which will be used in the game
    Map *map = init_map();

    Assets *assets = load_all_assets(renderer, map);

     
    PlayerState player_state = PLAYER_ON_MENU;

    bool user_wants_to_quit = false;
    while (!user_wants_to_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                user_wants_to_quit = true; 
            else if (event.type == SDL_KEYDOWN) 
                handle_keypress(event, &player_state, map, assets);
        }
        SDL_RenderClear(renderer);
    
        handle_state(player_state, map, assets);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60); 
    }

    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
