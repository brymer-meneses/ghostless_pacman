/*
 * This file contains functions that process and render the different 
 * states of the application.
 */

#include "stdio.h"
#include "stdbool.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

void render_state(States* states, Board *board, Assets *assets) {
    /* 
     *   A function that handles the `state` of the application, it changes
     *   `state` when the player hits a wall causing them to lose or when the 
     *   player chooses to go to the menu etc. This function `renders`, in other
     *   words, shows the current `state` of the application.
     *   
     *
     *   params: 
     *       States *states 
     *             A pointer to the struct `States`, which handles the different states of the
     *             game. 
     *       Board *board
     *             A pointer to the struct Board, which encapsulates variables required 
     *            in the game itself.
     *       Assets *assets
     *             A pointer to the struct `Assets`, which contains the images or sounds
     *             used in the application.
     */

    // Sprite is a struct that holds several variables that  
    
    Sprite *about_screen                 = &assets->misc.about_screen;
    Sprite *game_lost_hit_block          = &assets->prompt.game_lost_hit_block;
    Sprite *game_lost_hit_border         = &assets->prompt.game_lost_hit_border;
    Sprite *game_lost_insufficient_food  = &assets->prompt.game_lost_insufficient_food;
    Sprite *game_won                     = &assets->prompt.game_won;

    Sprite *player_chose_start           = &assets->menu.player_chose_start;
    Sprite *player_chose_about           = &assets->menu.player_chose_about;
    Sprite *player_chose_exit            = &assets->menu.player_chose_exit;
    Sprite *player_chose_none            = &assets->menu.player_chose_none;
    Sprite *player_chose_tutorial        = &assets->menu.player_chose_tutorial;

    Sprite *wrong_input_in_about_game    = &assets->reminders.wrong_input_in_about_game;
    Sprite *wrong_input_in_menu          = &assets->reminders.wrong_input_in_menu;
    Sprite *wrong_input_in_tutorial      = &assets->reminders.wrong_input_in_tutorial;
    Sprite *wrong_input_in_game          = &assets->reminders.wrong_input_in_game;
    Sprite *wrong_input_in_game_prompts  = &assets->reminders.wrong_input_in_game_prompts;
    Sprite *wrong_input_in_food_input    = &assets->reminders.wrong_input_in_food_input;

    Sprite *tutorial_pages               = assets->misc.tutorial_pages;
    Sprite *score_visuals                = assets->misc.score_visuals;
    Sprite *food_input_prompts           = assets->misc.food_input_prompts;

    // PlayerState refers to the location of the player.
    // This will change if the player goes to the menu or in the tutorial.
    enum PlayerState player_state               = states->player_state;

    // GameState refers to the state of the main game itself. Here "main game", 
    // refers to the time wherein the player can see the board along with the food
    // and pacman. This state tracks when the player won the game, lost etc.
    enum GameState game_state                   = states->game_state;

    // WrongInputState tracks when the player presses a wrong key.
    enum WrongInputState wrong_input_state      = states->wrong_input_state;

    // MenuChoiceState tracks the current selection of the player in the menu. This
    // is responsible for highlighting the current menu choice of the player.
    enum MenuChoiceState current_menu_choice    = states->current_menu_choice;

    // `current_tutorial_page` tracks the current `page` in the tutorial.
    int current_tutorial_page                   = states->current_tutorial_page;

    // The switch statement below
    switch (player_state) {
        case PLAYER_IN_MENU:
            switch (current_menu_choice) {
                case PLAYER_CHOSE_START:
                    render_sprite(player_chose_start);
                    break;
                case PLAYER_CHOSE_ABOUT:
                    render_sprite(player_chose_about);
                    break;
                case PLAYER_CHOSE_EXIT:
                    render_sprite(player_chose_exit);
                    break;
                case PLAYER_CHOSE_TUTORIAL:
                    render_sprite(player_chose_tutorial);
                    break;
                case PLAYER_CHOSE_NONE:
                    render_sprite(player_chose_none);
                }
            break;
        case PLAYER_IN_TUTORIAL:
            render_sprite(&tutorial_pages[current_tutorial_page]);
            break;
        case PLAYER_IN_ABOUT:
            render_sprite(about_screen);
            break;
        case PLAYER_IN_GAME:
            switch (game_state) {
                case GAME_IN_FOOD_NUMBER_INPUT:
                    // C is a zero based language that's why we have to subtract 1 
                    // when accessing the Sprite array `food_input_prompts`.
                    // This is also the reason why we have to 
                    // set the initial number of foods selected to 1, since it will be decremented to 0.
                    
                    render_sprite(&food_input_prompts[states->current_number_of_foods_picked - 1]);
                    break;
                case GAME_WON:
                    render_sprite(game_won);
                    render_sprite(&score_visuals[board->total_player_score]);
                    break;
                case GAME_LOST_HIT_BLOCK:
                    render_sprite(game_lost_hit_block);
                    render_sprite(&score_visuals[board->total_player_score]);
                    break;
                case GAME_LOST_HIT_BORDER:
                    render_sprite(game_lost_hit_border);
                    render_sprite(&score_visuals[board->total_player_score]);
                    break;
                case GAME_LOST_INSUFFICIENT_FOOD:
                    render_sprite(game_lost_insufficient_food);
                    render_sprite(&score_visuals[board->total_player_score]);
                    break;
                case GAME_IN_PROGRESS:
                    render_board(board, assets);
                    render_sprite(&score_visuals[board->total_player_score]);
                    break;
            }
            break;
    }
    
    // The switch statement below processes the wrong input states which were 
    // recorded and displays a prompt in response to these wrong inputs.
    switch (wrong_input_state) {
        case WRONG_INPUT_NONE:
            break;
        case WRONG_INPUT_IN_ABOUT_GAME:
            render_reminder(wrong_input_in_about_game, states, 3000, 535, 10);
            break;
        case WRONG_INPUT_IN_GAME:
            render_reminder(wrong_input_in_game, states, 3000, 535, 10);
            break;
        case WRONG_INPUT_IN_TUTORIAL:
            render_reminder(wrong_input_in_tutorial, states, 3000, 535, 10);
            break;
        case WRONG_INPUT_IN_FOOD_INPUT:
            render_reminder(wrong_input_in_food_input, states, 3000, 535, 10);
            break;
        case WRONG_INPUT_IN_MENU:
            render_reminder(wrong_input_in_menu, states, 3000, 535, 10);
            break;
        case WRONG_INPUT_IN_GAME_PROMPTS:
            render_reminder(wrong_input_in_game_prompts, states, 3000, 535, 10);
            break;
    }

    // Shoo the quit confirmation prompt.
    if (states->show_quit_confirmation) {
        render_sprite(&assets->misc.quit_confirmation_prompt);
    };
}

void process_keypress(SDL_Event event, States *states, Board* board, Assets* assets) {
    /* 
     *   A function that processes the different keypresses made by the player during the game.
     *   This function modifies the `states` variable in accordance with the
     *   response to the various keypresses of the player.
     *
     *   params: 
     *       SDL_Event event 
     *             Specifies an `event` in the game like when the player presses
     *             the button `w` on their keyboard.
     *       State *states
     *             A pointer to the struct `States`, which tracks all the states
     *             of the game.
     *       Board *board    
     *             A pointer to the struct `Board`, which encapsulates variables required 
     *             in the game.
     *       Assets *assets
     *             A pointer to the struct `Assets`, which holds all the images or sounds
     *             used in the game.
     */
    bool is_not_on_the_last_slide   = states->current_tutorial_page != NUMBER_OF_PAGES_IN_TUTORIAL - 1;
    bool is_not_on_the_first_slide  = states->current_tutorial_page != 0;

    enum PlayerState player_state               = states->player_state;
    enum GameState game_state                   = states->game_state;
    enum MenuChoiceState current_menu_choice    = states->current_menu_choice;

    SDL_Keycode player_keypress     = event.key.keysym.sym;

    // The following if-statement handles the state when the exit prompt is
    // shown in the screen.
    if (states->show_quit_confirmation) {
        switch (player_keypress) {
            case SDLK_n:
                states->show_quit_confirmation = false;
                break;
            case SDLK_y:
                states->player_wants_to_quit = true;
                break;
        }
        // Early return ensures that the application will ignore every other 
        // keypress aside from `n` and `y` when the quit confirmation prompt
        // is shown in the screen.
        return;
    }

    // NOTE:
    // SDLK_`x` refers to a keypress on the key `x`, while SDLK_KP_`x` refers
    // to a keypress on the key `x` that is located on the "keypad". This is
    // the reasoning why there is a double case to the switch statements
    // when the player is on the food input prompt.

    // NOTE:
    // The copious amount of switch statements were due to the fact that the application
    // had to take into consideration the different `states` that occur throughout the runtime
    // of the application. These `states` listen to different keys, this is why there is a need
    // to separate them.

    switch (player_state) {
        case PLAYER_IN_GAME:
            switch (game_state) {
                case GAME_IN_FOOD_NUMBER_INPUT:
                    // This case specifies that the player inputs a number from 2 to 9
                    // as the desired number of food pieces on the game. An option to return
                    // to menu is also offered by pressing 'M'. Otherwise, a wrong input
                    // reminder will be shown.
                    switch (player_keypress) {
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
                            // The following if-else statement handles when the
                            // user presses enter without entering their desired food number.
                            if (states->current_number_of_foods_picked != 1) {

                                init_board(board, assets, states->current_number_of_foods_picked);
                                states->game_state = GAME_IN_PROGRESS;
                                Mix_PlayChannel(1, assets->sounds.start_game, 0);

                            } else {

                                states->wrong_input_state = WRONG_INPUT_IN_FOOD_INPUT;
                                states->wrong_input_time = SDL_GetTicks();
                                Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                            }
                            // Reset the number of current foods picked
                            states->current_number_of_foods_picked = 1;
                            break;
                        default:
                            // Play the wrong input sound and register a wrong key state
                            Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                            states->wrong_input_state = WRONG_INPUT_IN_FOOD_INPUT;
                            states->wrong_input_time = SDL_GetTicks();
                            break;
                    }
                    break;
                case GAME_IN_PROGRESS:
                    // This case is associated with the accepted keypresses during the actual game.
                    // That is, 'W' to move up, 'S' to move down, 'A' to move left, and 'D' to move
                    // right. Also, options to return to menu and to exit the game can be done by
                    // pressing 'M' or pressing 'X' respectively. Keypresses othere than these would
                    // trigger a wrong input reminder.
                    switch (player_keypress) {
                        case SDLK_w:
                            move_pacman(MOVE_PACMAN_UP, assets, board, states);
                            break;
                        case SDLK_a:
                            move_pacman(MOVE_PACMAN_LEFT, assets, board, states);
                            break;
                        case SDLK_s:
                            move_pacman(MOVE_PACMAN_DOWN, assets, board, states);
                            break;
                        case SDLK_d:
                            move_pacman(MOVE_PACMAN_RIGHT, assets, board, states);
                            break;
                        case SDLK_m:
                            Mix_PlayChannel(0, assets->sounds.option_select, 0);
                            states->player_state = PLAYER_IN_MENU;
                            states->game_state = GAME_IN_FOOD_NUMBER_INPUT;
                        case SDLK_x:
                            Mix_PlayChannel(0, assets->sounds.quit_sound, 0);
                            states->show_quit_confirmation = true;
                            break;
                        default:
                            Mix_PlayChannel(0, assets->sounds.game_notification, 0);
                            states->wrong_input_state = WRONG_INPUT_IN_GAME;
                            states->wrong_input_time = SDL_GetTicks();
                            break;
                        }
                    break;
                // The following `game_states` all listen to the same
                // keypresses, that's why they are grouped to avoid
                // code duplication.
                case GAME_LOST_INSUFFICIENT_FOOD:
                case GAME_LOST_HIT_BORDER:
                case GAME_LOST_HIT_BLOCK:
                case GAME_WON:
                    switch (player_keypress) {
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
            switch (player_keypress) {
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
                    // A separate switch is created for pressing 'Enter'. This allows the
                    // user to change the chosen  option, even after pressing some other
                    // number previously. Once 'Enter' is pressed, the game state will be
                    // changed to GAME_IN_PROGRESS, which is the actual game proper.
                    switch (states->current_menu_choice) {
                        case PLAYER_CHOSE_ABOUT:
                            // Play confirmation sound
                            Mix_PlayChannel(1, assets->sounds.open_about_game, 0);
                            states->player_state = PLAYER_IN_ABOUT;
                            break;
                        case PLAYER_CHOSE_TUTORIAL:
                            // Play confirmation sound
                            Mix_PlayChannel(1, assets->sounds.option_confirm, 0);
                            states->player_state = PLAYER_IN_TUTORIAL;
                            break;
                        case PLAYER_CHOSE_START:
                            // Play confirmation sound
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
            states->current_tutorial_page = 0;
            break;
        case PLAYER_IN_TUTORIAL:
            switch (player_keypress) {
                case SDLK_RIGHT:
                    if (is_not_on_the_last_slide) 
                        states->current_tutorial_page += 1;
                        Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
                    break;
                case SDLK_LEFT:
                    if (is_not_on_the_first_slide)  
                        states->current_tutorial_page -= 1;
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
                    Mix_PlayChannel(0, assets->sounds.quit_sound, 0);
                    states->show_quit_confirmation = true;
                    break;
                default:
                    Mix_PlayChannel(-1, assets->sounds.game_notification, 0);
                    states->wrong_input_state = WRONG_INPUT_IN_TUTORIAL;
                    states->wrong_input_time = SDL_GetTicks();
                    break;
            }
            break;
        case PLAYER_IN_ABOUT:
            switch (player_keypress) {
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
    return;
}
