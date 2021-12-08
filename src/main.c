/*
 * This file, 'main.c', contains the main loop of the game as well as the
 * initialization of SDL and the termination of the game.
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

    init_SDL();

    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    // Initialize application icon 
    SDL_Surface* icon = IMG_Load("../assets/game_elements/pacman_solo.png");
    SDL_SetWindowIcon(window, icon);
    // ?

    // Set the background color to the color 
    SDL_SetRenderDrawColor(renderer, 24, 28, 36, 1);
    
    // Load all game assets
    Assets assets = load_all_assets(renderer);

    // Initialize map which will be used in the game
    Board board;
     
    States states;
    // Set the initial state of the player
    states.player_state = PLAYER_IN_MENU;
    // Set the initial state of the game when th player enters th game from 
    // the menu.
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
        // Clears the previous `states` in the window.
        SDL_RenderClear(renderer);
    
        render_state(&states, &board, &assets);

        SDL_RenderPresent(renderer);

        // Limit the framerate to 60 frames per second
        SDL_Delay(1000/60); 
    }

    free_all_assets(&assets);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    Mix_CloseAudio();
    return 0;
}
