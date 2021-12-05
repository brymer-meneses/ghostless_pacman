#include "stdio.h"
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
    Sprite game_lost_hit_block = assets->prompt.game_lost_hit_block;
    Sprite game_lost_hit_border = assets->prompt.game_lost_hit_border;
    Sprite game_lost_insufficient_food = assets->prompt.game_lost_insufficient_food;
    Sprite game_won = assets->prompt.game_won;
    Sprite* tutorial_slides = assets->misc.tutorial_slides;

    // states
    enum PlayerState player_state = states->player_state;
    enum GameState game_state =states->game_state;
    enum WrongKeyState wrong_key_state = states->wrong_key_state;


    int current_tutorial_slide_index = states->current_tutorial_slide_index;

    switch (player_state) {
        case PLAYER_IN_MENU:
            render_sprite(&homescreen);
            break;
        case PLAYER_IN_TUTORIAL:
            render_sprite(&tutorial_slides[current_tutorial_slide_index]);
            break;
        case PLAYER_IN_ABOUT:
            render_sprite(&about_screen);
            break;
        case PLAYER_IN_GAME:
            switch (game_state) {
                case GAME_IN_FOOD_NUMBER_INPUT:
                    break;
                case GAME_WON:
                    render_map(map, assets);
                    render_sprite(&game_won);
                    break;
                case GAME_LOST_HIT_BLOCK:
                    render_map(map, assets);
                    render_sprite(&game_lost_hit_block);
                    break;
                case GAME_LOST_HIT_BORDER:
                    render_map(map, assets);
                    render_sprite(&game_lost_hit_border);
                    break;
                case GAME_LOST_INSUFFICIENT_FOOD:
                    render_map(map, assets);
                    render_sprite(&game_lost_insufficient_food);
                    break;
                case GAME_IN_PROGRESS:
                    render_map(map, assets);
                    break;
            }
            break;
    }
    switch (wrong_key_state) {
        case WRONG_KEY_NONE:
            // do nothing
            break;
        case WRONG_KEY_IN_ABOUT_GAME:
            break;
        case WRONG_KEY_IN_GAME:
            break;
        case WRONG_KEY_IN_TUTORIAL:
            // here
            break;
        case WRONG_KEY_IN_FOOD_INPUT:
            break;
        case WRONG_KEY_IN_MENU:
            break;
        case WRONG_KEY_IN_GAME_PROMPTS:
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

    enum PlayerState player_state = states->player_state;
    enum GameState game_state = states->game_state;

    switch (player_state) {
        case PLAYER_IN_GAME:
            switch (game_state) {
                case GAME_IN_FOOD_NUMBER_INPUT:
                    break;
                case GAME_IN_PROGRESS:
                    switch (event.key.keysym.scancode) {
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
                            states->wrong_key_state = WRONG_KEY_IN_GAME;
                            break;
                        }
                    break;
                case GAME_LOST_INSUFFICIENT_FOOD:
                case GAME_LOST_HIT_BORDER:
                case GAME_LOST_HIT_BLOCK:
                case GAME_WON:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_R:
                            states->player_state = PLAYER_IN_GAME;
                            // DEBUG: change this to GAME_IN_FOOD_NUMBER_INPUT
                            states->game_state = GAME_IN_PROGRESS;
                            reset_map(map, assets);
                            break;
                        case SDL_SCANCODE_M:
                            states->player_state = PLAYER_IN_MENU;
                            // DEBUG: change this to GAME_IN_FOOD_NUMBER_INPUT
                            states->game_state = GAME_IN_PROGRESS;
                            reset_map(map, assets);
                            break;
                        case SDL_SCANCODE_X:
                            // quit box
                            break;
                        default:
                            states->wrong_key_state = WRONG_KEY_IN_GAME_PROMPTS;
                            break;
                        }
                        break;
            };
            break;
        case PLAYER_IN_MENU:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_0:
                    states->player_state = PLAYER_IN_GAME;
                    break;
                case SDL_SCANCODE_A:
                    states->player_state = PLAYER_IN_ABOUT;
                    break;
                case SDL_SCANCODE_1:
                    states->player_state = PLAYER_IN_TUTORIAL;
                    break;
                default:
                    states->wrong_key_state = WRONG_KEY_IN_MENU;
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
                        Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
                    break;
                case SDL_SCANCODE_LEFT:
                    if (is_not_on_the_first_slide)  
                        states->current_tutorial_slide_index -= 1;
                        Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
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
                    states->wrong_key_state = WRONG_KEY_IN_TUTORIAL;
                    break;
            }
            break;
        case PLAYER_IN_ABOUT:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_M:
                    states->player_state = PLAYER_IN_MENU;
                    break;
                default:
                    states->wrong_key_state = WRONG_KEY_IN_ABOUT_GAME;
                }
        default:
            break;
    }

}

