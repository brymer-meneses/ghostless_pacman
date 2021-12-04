#include "stdbool.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"



void handle_state(States* states, Map *map, Assets *assets) {
    /* 
     *   A function that handles the `state` of the game, the game changes
     *   `state` when the player hits a wall causing them to lose. This function handles
     *   the different `states` of the game.
     *
     *   params: 
     *       SDL_Event event 
     *           - Specifies an `event` in the game like when the player presses
     *             the button `w` on their keyboard.
     *       PlayerState *player_state
     *           - A pointer to an enum `PlayerState` which tracks the state of the application. 
     *             It is necessay to become a pointer since this function will modify some 
     *       Map *map    
     *          - A pointer to the struct Map, which encapsulates variables required 
     *            in the game.
     *       GameSprites *game_sprites
     *           - A pointer to the struct `GameSprites` which holds all the sprites for running the game.
     */

    Sprite homescreen = assets->misc.homescreen;
    Sprite about_screen = assets->misc.about_screen;
    Sprite player_lost_hit_block = assets->prompt.player_lost_hit_block;
    Sprite player_lost_hit_border = assets->prompt.player_lost_hit_border;
    Sprite player_lost_insufficient_food = assets->prompt.player_lost_insufficient_food;
    Sprite player_won = assets->prompt.player_won;

    PlayerState player_state = states->player_state;
    int current_tutorial_slide_index = states->current_tutorial_slide_index;

    switch (player_state) {
        case PLAYER_IN_MENU:
            render_sprite(&homescreen);
            break;
        case PLAYER_IN_TUTORIAL:
            render_sprite(&assets->misc.tutorial_slides[current_tutorial_slide_index]);
            break;
        case PLAYER_IN_ABOUT:
            render_sprite(&about_screen);
            break;
        case PLAYER_IN_GAME:
            render_map(map, assets);
            break;
        case PLAYER_WON:
            render_map(map, assets);
            render_sprite(&player_won);
            break;
        case PLAYER_LOST_HIT_BLOCK:
            render_map(map, assets);
            render_sprite(&player_lost_hit_block);
            break;
        case PLAYER_LOST_HIT_BORDER:
            render_map(map, assets);
            render_sprite(&player_lost_hit_border);
            break;
        case PLAYER_LOST_INSUFFICIENT_FOOD:
            render_map(map, assets);
            render_sprite(&player_lost_insufficient_food);
            break;
    }
}

void handle_keypress(SDL_Event event, States *states, Map* map, Assets* assets) {
    /* 
     *   A function that handles the various keypresses of the user.
     *
     *   params: 
     *       SDL_Event event 
     *           - Specifies an `event` in the game like when the player presses
     *             the button `w` on their keyboard.
     *       PlayerState *player_state
     *           - A pointer to an enum `PlayerState` which tracks the state of the application. 
     *             It is necessay to become a pointer since this function will modify some 
     *       Map *map    
     *          - A pointer to the struct Map, which encapsulates variables required 
     *            in the game.
     *       GameSprites *game_sprites
     *           - A pointer to the struct `GameSprites` which holds all the sprites for running the game.
     */
    bool is_not_on_the_last_slide = states->current_tutorial_slide_index != NUMBER_OF_TUTORIAL_SLIDES - 1;
    bool is_not_on_the_first_slide = states->current_tutorial_slide_index != 0;

    PlayerState player_state = states->player_state;
    switch (player_state) {
        case PLAYER_IN_GAME:
            switch (event.key.keysym.scancode) {
                // Moving pacman
                case SDL_SCANCODE_W:
                    move_pacman(MOVE_UP, assets, map, states);
                    break;
                case SDL_SCANCODE_A:
                    move_pacman(MOVE_LEFT, assets, map, states);
                    break;
                case SDL_SCANCODE_S:
                    move_pacman(MOVE_DOWN, assets, map, states);
                    break;
                case SDL_SCANCODE_D:
                    move_pacman(MOVE_RIGHT, assets, map, states);
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    // reset the map
                    reset_map(map, assets);
                default:
                    // TODO: Warn the user if the input is invalid
                    // perhaps add some instructions?
                    break;
                }
            break;
        case PLAYER_IN_MENU:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_0:
                    states->player_state = PLAYER_IN_GAME;
                    break;
                case SDL_SCANCODE_1:
                    *player_state = PLAYER_ON_TUTORIAL;
                    break;
                case SDL_SCANCODE_A:
<<<<<<< HEAD
                    *player_state = PLAYER_ON_ABOUT;
=======
                    states->player_state = PLAYER_IN_ABOUT;
                    break;
                case SDL_SCANCODE_1:
                    states->player_state = PLAYER_IN_TUTORIAL;
>>>>>>> 6481d5c (dump commits for now)
                    break;
                default:
                    // TODO: Warn the user if the input is invalid
                    // perhaps add some instructions?
                    break;
            };
            // reset tutorial slide
            states->current_tutorial_slide_index = 0;
            break;
        case PLAYER_IN_TUTORIAL:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_RIGHT:
                    if (is_not_on_the_last_slide) 
                        states->current_tutorial_slide_index += 1;
                    break;
                case SDL_SCANCODE_LEFT:
                    if (is_not_on_the_first_slide)  
                        states->current_tutorial_slide_index -= 1;
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                case SDL_SCANCODE_0:
                    states->player_state = PLAYER_IN_GAME;
                    break;
                // handle exit
                case SDL_SCANCODE_2:
                    break;
                default:
                    // TODO: Warn the user if the input is invalid
                    // perhaps add some instructions?
                    break;
            }
            break;
        case PLAYER_IN_ABOUT:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                }
        case PLAYER_WON:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_R:
                    states->player_state = PLAYER_IN_GAME;
                    reset_map(map, assets);
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                case SDL_SCANCODE_X:
                    // quit box
                    break;
                }
        case PLAYER_LOST_HIT_BLOCK:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_R:
                    states->player_state = PLAYER_IN_GAME;
                    reset_map(map, assets);
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    reset_map(map, assets);
                    break;
                case SDL_SCANCODE_X:
                    // quit box
                    break;
                }
        case PLAYER_LOST_HIT_BORDER:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_R:
                    states->player_state = PLAYER_IN_GAME;
                    reset_map(map, assets);
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                case SDL_SCANCODE_X:
                    // quit box
                    break;
                }
        case PLAYER_LOST_INSUFFICIENT_FOOD:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_R:
                    states->player_state = PLAYER_IN_GAME;
                    reset_map(map, assets);
                    break;
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                case SDL_SCANCODE_X:
                    // quit box
                    break;
                }
        default:
            break;
    }

}

