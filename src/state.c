#include "stdbool.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"



void handle_state( PlayerState state, Map *map, GameSprites* game_sprites, MiscSprites* misc_sprites, PromptSprites* prompt_sprites) {
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
    Sprite homescreen = misc_sprites->homescreen;
    switch (state) {
        case PLAYER_ON_MENU:
            render_sprite(&homescreen);
            break;
        case PLAYER_ON_TUTORIAL:
            break;
        case PLAYER_ON_ABOUT:
            break;
        case PLAYER_STILL_PLAYING:
            render_map(map, game_sprites);
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

void handle_keypress(SDL_Event event, PlayerState *player_state, Map* map ,GameSprites* game_sprites) {
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
                    move_pacman(MOVE_UP, game_sprites, map);
                    break;
                case SDL_SCANCODE_A:
                    move_pacman(MOVE_LEFT, game_sprites, map);
                    break;
                case SDL_SCANCODE_S:
                    move_pacman(MOVE_DOWN, game_sprites, map);
                    break;
                case SDL_SCANCODE_D:
                    move_pacman(MOVE_RIGHT, game_sprites, map);
                    break;
                case SDL_SCANCODE_M:
                    *player_state = PLAYER_ON_MENU;
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

GameSprites load_all_game_sprites(SDL_Renderer *renderer, Map* map) {
    /* 
     *   A function that loads all the sprites for running the game itself.
     *
     *   params: 
     *      SDL_Renderer *renderer
     *          - A pointer to the SDL_Renderer, which handles the rendering of
     *            elements in the screen
     *      Map *map    
     *          - A pointer to the struct Map, which encapsulates variables required 
     *            in the game.
     */
    SDL_Rect pacman_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "../assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {GRID_POSITION_X, GRID_POSITION_Y, GRID_SIZE, GRID_SIZE};
    Sprite grid = load_sprite(renderer, "../assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {GRID_POSITION_X, GRID_POSITION_Y, 23, 33};
    Sprite exit = load_sprite(renderer, "../assets/exit.png", 0, 1, exit_rect);


    SDL_Rect block_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=27, .w=23};

    GameSprites sprites;
    sprites.pacman = pacman;
    sprites.grid = grid;
    sprites.exit = exit;

    sprites.blocks = (Sprite*) malloc(map->number_of_blocks * sizeof(Sprite));
    sprites.foods = (Sprite*) malloc(map->number_of_foods * sizeof(Sprite));

    for (int i=0; i<map->number_of_blocks; i++) {
        sprites.blocks[i] = load_sprite(renderer, "../assets/box.png", 0 , 1, block_rect);
    }

    for (int i=0; i<map->number_of_foods; i++) {
        sprites.foods[i] = load_sprite(renderer, "../assets/food.png", 0 , 1, food_rect);
    }

    return sprites;
}

PromptSprites load_all_prompt_sprites(SDL_Renderer* renderer) {
    /* 
     *   A function that loads all the sprites responsible for prompts,
     *
     *   params: 
     *      SDL_Renderer *renderer
     *          - A pointer to the SDL_Renderer, which handles the rendering of
     *            elements in the screen
     *   returns:
     *      PromptSprites
     *          - A struct that encapsulates all the sprites required for popups and prompts. 
     */
    PromptSprites prompt_sprites;

    return prompt_sprites;
}

MiscSprites load_all_misc_sprites(SDL_Renderer* renderer) {
    /* 
     *   A function that loads all the miscellanious sprites required in the game
     *
     *   params: 
     *      SDL_Renderer *renderer
     *          - A pointer to the SDL_Renderer, which handles the rendering of
     *            elements in the screen
     *
     *   returns:
     *      MusicSprites
     *          - A struct that encapsulates all the miscellaneous sprites.
     */
    MiscSprites misc_sprites;

    SDL_Rect home_screen_rect = {0, 0, 640, 640};
    Sprite home_screen = load_sprite(renderer, "../assets/homescreen.png", 0, 1, home_screen_rect);


    misc_sprites.homescreen = home_screen;
    return misc_sprites;
}

