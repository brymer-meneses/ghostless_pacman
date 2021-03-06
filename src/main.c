/*
 * This file, 'main.c', contains the main loop of the game,
 * as well as its termination and the initialization of SDL
 *
 * Authors
 *      Brymer Meneses 
 *      Aaron Paul Gapud
 */


// Standard Library Imports
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"

// Project-wide Headers
#include "utils.h"
#include "logic.h"
#include "state.h"
#include "assets.h"
#include "declarations.h"


int main () {

    // Set a seed for the random number generator
    // that is used in the game.
    time_t t;
    srand((unsigned) time(&t));

    // Initialize SDL
    init_SDL();

    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    // Set the background color for the window
    SDL_SetRenderDrawColor(renderer, 24, 28, 36, 1);
    
    // Load all application assets
    Assets assets = load_all_assets(renderer);

    // Initialize board that will be used in the game
    Board board;
    
    States states;
    // Set the initial state of the player
    states.player_state = PLAYER_IN_MENU;
    // Set the initial state of the game when
    // the player starts the game from the menu
    states.game_state = GAME_IN_FOOD_NUMBER_INPUT;
    // Set the initial index for the tutorial page
    states.current_tutorial_page = 0;
    // Set the initial wrong key state
    states.wrong_input_state = WRONG_INPUT_NONE,
    // Set the initial number of foods. This value will change later on.
    states.current_number_of_foods_picked = 1;
    // Set the initial menu choice
    states.current_menu_choice = PLAYER_CHOSE_NONE;

    states.show_quit_confirmation = false;
    states.player_wants_to_quit = false;

    // Play background music
    Mix_FadeInMusic(assets.sounds.background_music, -1, 4000);

    while (!states.player_wants_to_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                states.show_quit_confirmation = true;
            }
            else if (event.type == SDL_KEYDOWN) 
                process_keypress(event, &states, &board, &assets);
        }
        // Clear the previous `states` in the window.
        SDL_RenderClear(renderer);
    
        // Render the current `state` of the application
        render_state(&states, &board, &assets);
        SDL_RenderPresent(renderer);

        // Limit the framerate to 60 frames per second
        SDL_Delay(1000/60); 
    }

    // Free all resources and allocated memory used throughout
    // the application
    free_all_assets(&assets);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
