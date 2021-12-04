#include "stdbool.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"



void handle_state( PlayerState state, Map *map, Assets *assets) {
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
     *      Map *map    
     *          - A pointer to the struct Map, which encapsulates variables required 
     *            in the game.
     *       GameSprites *game_sprites
     *           - A pointer to the struct `GameSprites` which holds all the sprites for running the game.
     */
    Sprite homescreen = assets->misc.homescreen;
    switch (state) {
        case PLAYER_ON_MENU:
            render_sprite(&homescreen);
            break;
        case PLAYER_ON_TUTORIAL:
            break;
        case PLAYER_ON_ABOUT:
            break;
        case PLAYER_STILL_PLAYING:
            render_map(map, assets);
            break;
        case PLAYER_WON:
            break;
        case PLAYER_LOST_HIT_BLOCK:
            break;
        case PLAYER_LOST_HIT_BORDER:
            break;
        case PLAYER_LOST_INSUFFICIENT_FOOD:
            break;
    }
}

void handle_keypress(SDL_Event event, PlayerState *player_state, Map* map, Assets* assets) {
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
     *      Map *map    
     *          - A pointer to the struct Map, which encapsulates variables required 
     *            in the game.
     *       GameSprites *game_sprites
     *           - A pointer to the struct `GameSprites` which holds all the sprites for running the game.
     */
    switch (*player_state) {
        case PLAYER_STILL_PLAYING:
            switch (event.key.keysym.scancode) {
                // Moving pacman
                case SDL_SCANCODE_W:
                    move_pacman(MOVE_UP, assets, map);
                    break;
                case SDL_SCANCODE_A:
                    move_pacman(MOVE_LEFT, assets, map);
                    break;
                case SDL_SCANCODE_S:
                    move_pacman(MOVE_DOWN, assets, map);
                    break;
                case SDL_SCANCODE_D:
                    move_pacman(MOVE_RIGHT, assets, map);
                    break;
                case SDL_SCANCODE_M:
                    *player_state = PLAYER_ON_MENU;
                    // reset the map
                    reset_map(map, assets);
                default:
                    // TODO: Warn the user if the input is invalid
                    // perhaps add some instructions?
                    break;
                }
            break;
        case PLAYER_ON_MENU:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_S:
                    *player_state = PLAYER_STILL_PLAYING;
                    break;
                default:
                    // TODO: Warn the user if the input is invalid
                    // perhaps add some instructions?
                    break;
            }
        case PLAYER_ON_TUTORIAL:
            break;
        default:
            break;
    }

}

