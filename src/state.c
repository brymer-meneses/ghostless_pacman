#include "stdio.h"
#include "stdbool.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

void render_state(States* states, Map *map, Assets *assets) {
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

    Sprite about_screen = assets->misc.about_screen;
    Sprite game_lost_hit_block = assets->prompt.game_lost_hit_block;
    Sprite game_lost_hit_border = assets->prompt.game_lost_hit_border;
    Sprite game_lost_insufficient_food = assets->prompt.game_lost_insufficient_food;
    Sprite game_won = assets->prompt.game_won;
    Sprite* tutorial_slides = assets->misc.tutorial_slides;

    // states
    enum PlayerState player_state = states->player_state;
    enum GameState game_state =states->game_state;
    enum WrongInputState wrong_input_state = states->wrong_input_state;
    enum MenuChoiceState current_menu_choice = states->current_menu_choice;

    int current_tutorial_slide_index = states->current_tutorial_slide_index;

    switch (player_state) {
        case PLAYER_IN_MENU:
            switch (current_menu_choice) {
                case PLAYER_CHOSE_START:
                    render_sprite(&assets->menu.player_chose_start);
                    break;
                case PLAYER_CHOSE_ABOUT:
                    render_sprite(&assets->menu.player_chose_about);
                    break;
                case PLAYER_CHOSE_EXIT:
                    render_sprite(&assets->menu.player_chose_exit);
                    break;
                case PLAYER_CHOSE_TUTORIAL:
                    render_sprite(&assets->menu.player_chose_tutorial);
                    break;
                case PLAYER_CHOSE_NONE:
                    render_sprite(&assets->menu.player_chose_none);
                }
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
                    // C is a zero based language that's why we have to subtract 1 
                    // when accessing its array. This is also the reason why we have to 
                    // set the initial number of foods selected to 1, since it will be decremented to 0.
                    render_sprite(&assets->misc.food_input_prompts[states->current_number_of_foods_picked-1]);
                    break;
                case GAME_WON:
                    render_map(map, assets);
                    render_sprite(&game_won);
                    render_sprite(&assets->misc.score_visuals[map->total_player_score]);
                    break;
                case GAME_LOST_HIT_BLOCK:
                    render_map(map, assets);
                    render_sprite(&game_lost_hit_block);
                    render_sprite(&assets->misc.score_visuals[map->total_player_score]);
                    break;
                case GAME_LOST_HIT_BORDER:
                    render_map(map, assets);
                    render_sprite(&game_lost_hit_border);
                    render_sprite(&assets->misc.score_visuals[map->total_player_score]);
                    break;
                case GAME_LOST_INSUFFICIENT_FOOD:
                    render_map(map, assets);
                    render_sprite(&game_lost_insufficient_food);
                    render_sprite(&assets->misc.score_visuals[map->total_player_score]);
                    break;
                case GAME_IN_PROGRESS:
                    render_map(map, assets);
                    render_sprite(&assets->misc.score_visuals[map->total_player_score]);
                    break;
            }
            break;
    }
    switch (wrong_input_state) {
        case WRONG_INPUT_NONE:
            // do nothing
            break;
        case WRONG_INPUT_IN_ABOUT_GAME:
            render_reminder(&assets->reminders.wrong_input_in_about_game, states, 2000, 535, 5);
            break;
        case WRONG_INPUT_IN_GAME:
            render_reminder(&assets->reminders.wrong_input_in_game, states, 2000, 535, 5);
            break;
        case WRONG_INPUT_IN_TUTORIAL:
            render_reminder(&assets->reminders.wrong_input_in_tutorial, states, 2000, 535, 5);
            break;
        case WRONG_INPUT_IN_FOOD_INPUT:
            render_reminder(&assets->reminders.wrong_input_in_food_input, states, 2000, 535, 5);
            break;
        case WRONG_INPUT_IN_MENU:
            render_reminder(&assets->reminders.wrong_input_in_menu, states, 2000, 535, 5);
            break;
        case WRONG_INPUT_IN_GAME_PROMPTS:
            render_reminder(&assets->reminders.wrong_input_in_game_prompts, states, 2000, 535, 5);
            break;
    }
    if (states->show_quit_confirmation) {
        render_sprite(&assets->misc.quit_confirmation_prompt);
    };
}

void register_keypress(SDL_Event event, States *states, Map* map, Assets* assets) {
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
    enum MenuChoiceState current_menu_choice = states->current_menu_choice;

    switch (player_state) {
        case PLAYER_IN_GAME:
            switch (game_state) {
                case GAME_IN_FOOD_NUMBER_INPUT:
                    switch (event.key.keysym.sym) {
                        case SDLK_2:
                        case SDLK_KP_2:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 2;
                            break;
                        case SDLK_3:
                        case SDLK_KP_3:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 3;
                            break;
                        case SDLK_4:
                        case SDLK_KP_4:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 4;
                            break;
                        case SDLK_5:
                        case SDLK_KP_5:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 5;
                            break;
                        case SDLK_6:
                        case SDLK_KP_6:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 6;
                            break;
                        case SDLK_7:
                        case SDLK_KP_7:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 7;
                            break;
                        case SDLK_8:
                        case SDLK_KP_8:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 8;
                            break;
                        case SDLK_9:
                        case SDLK_KP_9:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->current_number_of_foods_picked = 9;
                            break;
                        case SDLK_m:
                            Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                            states->player_state = PLAYER_IN_MENU;
                            states->current_number_of_foods_picked = 1;
                            break;
                        case SDLK_RETURN:
                            break;
                        default:
                            Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                            states->wrong_input_state = WRONG_INPUT_IN_FOOD_INPUT;
                            states->wrong_input_time = SDL_GetTicks();
                    }
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_RETURN:
                            Mix_PlayChannel(1, assets->sounds.start_game, 0);
                            if (states->current_number_of_foods_picked != 1) {
                                reset_map(map, assets, states->current_number_of_foods_picked);
                                states->game_state = GAME_IN_PROGRESS;
                            }
                            states->current_number_of_foods_picked = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case GAME_IN_PROGRESS:
                    switch (event.key.keysym.sym) {
                        case SDLK_w:
                            move_pacman(MOVE_UP, assets, map, states);
                            break;
                        case SDLK_a:
                            move_pacman(MOVE_LEFT, assets, map, states);
                            break;
                        case SDLK_s:
                            move_pacman(MOVE_DOWN, assets, map, states);
                            break;
                        case SDLK_d:
                            move_pacman(MOVE_RIGHT, assets, map, states);
                            break;
                        case SDLK_m:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->player_state = PLAYER_IN_MENU;
                            states->game_state = GAME_IN_FOOD_NUMBER_INPUT;
                        default:
                            Mix_PlayChannel(0, assets->sounds.game_notification, 0);
                            states->wrong_input_state = WRONG_INPUT_IN_GAME;
                            states->wrong_input_time = SDL_GetTicks();
                            break;
                        }
                    break;
                case GAME_LOST_INSUFFICIENT_FOOD:
                case GAME_LOST_HIT_BORDER:
                case GAME_LOST_HIT_BLOCK:
                case GAME_WON:
                    switch (event.key.keysym.sym) {
                        case SDLK_r:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->player_state = PLAYER_IN_GAME;
                            states->game_state = GAME_IN_FOOD_NUMBER_INPUT;
                            break;
                        case SDLK_m:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->player_state = PLAYER_IN_MENU;
                            states->game_state = GAME_IN_FOOD_NUMBER_INPUT;
                            break;
                        case SDLK_x:
                            states->show_quit_confirmation = true;
                            Mix_PlayChannel(0, assets->sounds.quit_sound, 0);
                            break;
                        default:
                            states->wrong_input_state = WRONG_INPUT_IN_GAME_PROMPTS;
                            states->wrong_input_time = SDL_GetTicks();
                            break;
                        }
                        break;
                    break;
            };
            break;
        case PLAYER_IN_MENU:
            switch (event.key.keysym.sym) {
                case SDLK_1:
                case SDLK_KP_1:
                    Mix_PlayChannel(0, assets->sounds.option_select, 0);
                    states->current_menu_choice = PLAYER_CHOSE_START;
                    break;
                case SDLK_a:
                    Mix_PlayChannel(0, assets->sounds.option_select, 0);
                    states->current_menu_choice = PLAYER_CHOSE_ABOUT;
                    break;
                case SDLK_2:
                case SDLK_KP_2:
                    Mix_PlayChannel(0, assets->sounds.option_select, 0);
                    states->current_menu_choice = PLAYER_CHOSE_TUTORIAL;
                    break;
                case SDLK_3:
                case SDLK_KP_3:
                    Mix_PlayChannel(0, assets->sounds.option_select, 0);
                    states->current_menu_choice = PLAYER_CHOSE_EXIT;
                    break;
                case SDLK_RETURN:
                    switch (states->current_menu_choice) {
                        case PLAYER_CHOSE_ABOUT:
                            Mix_PlayChannel(1, assets->sounds.open_about_game, 0);
                            states->player_state = PLAYER_IN_ABOUT;
                            break;
                        case PLAYER_CHOSE_TUTORIAL:
                            Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                            states->player_state = PLAYER_IN_TUTORIAL;
                            break;
                        case PLAYER_CHOSE_START:
                            Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                            states->player_state = PLAYER_IN_GAME;
                            break;
                        case PLAYER_CHOSE_EXIT:
                            states->show_quit_confirmation = true;
                            break;
                        case PLAYER_CHOSE_NONE:
                            break;
                    }
                    // Reset menu choice selection
                    states->current_menu_choice = PLAYER_CHOSE_NONE;
                    break;
                default:
                    Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                    states->wrong_input_state = WRONG_INPUT_IN_MENU;
                    states->wrong_input_time = SDL_GetTicks();
                    break;
            };
            // Reset tutorial slide
            states->current_tutorial_slide_index = 0;
            break;
        case PLAYER_IN_TUTORIAL:
            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    if (is_not_on_the_last_slide) 
                        states->current_tutorial_slide_index += 1;
                        Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
                    break;
                case SDLK_LEFT:
                    if (is_not_on_the_first_slide)  
                        states->current_tutorial_slide_index -= 1;
                        Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
                    break;
                case SDLK_m:
                    Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                    states->player_state = PLAYER_IN_MENU;
                    break;
                case SDLK_1:
                    Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                    states->player_state = PLAYER_IN_GAME;
                    break;
                case SDLK_x:
                    break;
                default:
                    Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                    states->wrong_input_state = WRONG_INPUT_IN_TUTORIAL;
                    states->wrong_input_time = SDL_GetTicks();
                    break;
            }
            break;
        case PLAYER_IN_ABOUT:
            switch (event.key.keysym.sym) {
                case SDLK_m:
                    Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                    states->player_state = PLAYER_IN_MENU;
                    break;
                default:
                    Mix_PlayChannel(0, assets->sounds.game_notification, 0);
                    states->wrong_input_state = WRONG_INPUT_IN_ABOUT_GAME;
                    states->wrong_input_time = SDL_GetTicks();
                }
        default:
            break;
    }

    if (states->show_quit_confirmation) {
        switch (event.key.keysym.sym) {
            case SDLK_n:
                states->show_quit_confirmation = false;
                break;
            case SDLK_y:
                states->is_game_running = false;
                break;
        }

    }
}
