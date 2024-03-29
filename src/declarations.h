/*
 *  This file, 'declarations.h', is where all the type defined structs, 
 *  enums and constant macros are located. 
 */

#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 640

#define SPRITE_UPDATE_DELAY 100

#define NUMBER_OF_BLOCKS 10
#define NUMBER_OF_PAGES_IN_TUTORIAL 7
#define NUMBER_OF_FOOD_INPUT_PROMPTS 9
#define NUMBER_OF_SCORE_VISUALS 10
#define MAX_NUMBER_OF_FOOD 9

// Defines the size (in pixels) of each box in the grid, which appears in the game
// itself.
#define BOX_WIDTH 45

/*
 *  MAX_ADJACENT_IMPASSABLE_NEIGHBORS is the maximum number of
 *  adjacent spaces around a food piece or an exit that is impossible
 *  to be passed through. This includes its adjacent blocks or borders.
 *
 *  Legend: 
 *       x  block
 *       *  food
 *       -  border
 *
 *  Example:
 *      x
 *       *
 *      ---
 *  This amounts to 4 impassable_adjacent_neighbors. 
 */
#define MAX_ADJACENT_IMPASSABLE_NEIGHBORS 3

/*
 * ELEMENT_INITIAL_POSITION_X and ELEMENT_INITIAL_POSITION_Y
 * is the initial position of a BoardElement.
 */
#define ELEMENT_INITIAL_POSITION_X (95 + 12)
#define ELEMENT_INITIAL_POSITION_Y (95 + 6)

#include "stdbool.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"

/*
 * BoardElement is an enum that encodes all the possible 
 * elements in the board.
 */

enum BoardElement {
    EMPTY,
    PACMAN,
    FOOD,
    BLOCK,
    EXIT,
};

/*
 * BoardPosition is a struct that holds the `x` and `y` position of a
 * BoardElement.
 */
typedef struct BoardPosition {
    int row;
    int col;
} BoardPosition;


/*
 * Sprite is a struct that holds related variables 
 * which are necessary for displaying images in the window
 */
typedef struct Sprite {
    SDL_Renderer        *renderer;
    SDL_Texture         *texture;
    SDL_Rect            rect;
    SDL_RendererFlip    flip;
    float               rotation;
    int                 total_frames;
    int                 frame_size;
    int                 current_frame;
} Sprite;

/*
 * Assets is a struct that holds the all the `assets` of the application. This 
 * includes images, or sounds which enhance the experience of the user. 
 */
typedef struct Assets {

    struct {
        Sprite pacman;
        Sprite main;
        Sprite exit;
        Sprite blocks[NUMBER_OF_BLOCKS];
        Sprite foods[MAX_NUMBER_OF_FOOD];
    } game;


    struct { 
        Sprite tutorial_pages[NUMBER_OF_PAGES_IN_TUTORIAL];
        Sprite food_input_prompts[NUMBER_OF_FOOD_INPUT_PROMPTS];
        Sprite score_visuals[NUMBER_OF_SCORE_VISUALS];
        Sprite about_screen;
        Sprite quit_confirmation_prompt;
    } misc;

    struct {
        Sprite player_chose_start;
        Sprite player_chose_tutorial;
        Sprite player_chose_exit;
        Sprite player_chose_about;
        Sprite player_chose_none;
    } menu;

    struct { 
        Sprite game_won;
        Sprite game_lost_hit_block;
        Sprite game_lost_hit_border;
        Sprite game_lost_insufficient_food;
    } prompt;

    struct {
        Sprite wrong_input_in_game;
        Sprite wrong_input_in_game_prompts;
        Sprite wrong_input_in_menu;
        Sprite wrong_input_in_about_game;
        Sprite wrong_input_in_food_input;
        Sprite wrong_input_in_tutorial;
    } reminders;

    // Struct for sounds
    struct { 
        Mix_Music* background_music;
        Mix_Chunk* pacman_munch;
        Mix_Chunk* pacman_step;
        Mix_Chunk* option_select;
        Mix_Chunk* option_confirm;
        Mix_Chunk* quit_sound;
        Mix_Chunk* open_about_game;
        Mix_Chunk* start_game;
        Mix_Chunk* game_notification;
        Mix_Chunk* game_win;
        Mix_Chunk* game_over;
    } sounds;


} Assets;

/*
 * Board is a struct that holds variables
 * related to the game itself.
 */

typedef struct Board { 
    // Pointer to a 10 by 10, two-dimensional array that encodes
    // the different positions of all elements in the board.
    enum BoardElement array[10][10];
    // The total number of foods generated in the board
    int number_of_foods;
    // Total total number of blocks generated in the board
    int number_of_blocks;
    // Current score of the player 
    int total_player_score;
} Board;


/* 
 * PlayerState specifies the following current states of the user:
 *     PLAYER_IN_MENU
 *        - where the player can choose from the
 *          options 'Start', 'Tutorial', 'About
 *          the Game', and 'Exit'
 *    PLAYER_IN_TUTORIAL
 *        - where the player is instructed on
 *          how to play the Pacman game
 *    PLAYER_IN_ABOUT
 *        - where proper acknowledgement and
 *          credits are given space
 *    PLAYER_IN_GAME
 *        - where the game is initiated
 */
enum PlayerState {
    PLAYER_IN_GAME,
    PLAYER_IN_MENU,
    PLAYER_IN_TUTORIAL,
    PLAYER_IN_ABOUT,
};

/* 
 * GameState specifies the following, given that the player state is in-game:
 *
 *   GAME_IN_FOOD_NUMBER_INPUT
 *       - where the player is prompted to choose the number of food
 *         to be displayed in the game board (from 2 to 9)
 *
 *   GAME_IN_PROGRESS
 *       - where the player plays the game
 *
 *   GameState also covers the results of the game:
 *
 *       GAME_LOST_HIT_BLOCK
 *       GAME_LOST_HIT_BORDER
 *       GAME_LOST_INSUFFICIENT_FOOD
 *       GAME_WON
 *
 *       Results can be one of the following:
 *            Game Won
 *                - (1) when Pacman reaches the door after eating all food pieces,
 *                      without hitting a block or moving out of the border
 *            Game Lost
 *                - (2) when Pacman hits a block
 *                - (3) when Pacman moves out of the border
 *                - (4) when Pacman reaches the door,
 *                   without eating all the food pieces
 */
enum GameState { 
    GAME_WON,
    GAME_LOST_HIT_BLOCK,
    GAME_LOST_HIT_BORDER,
    GAME_LOST_INSUFFICIENT_FOOD,
    GAME_IN_PROGRESS,
    GAME_IN_FOOD_NUMBER_INPUT,
};

/* 
 * WrongInputState is associated with the events where the input of the user is
 * not included to the certain keypresses accepted on some instances, such as:
 *
 *     In Menu
 *
 *         WRONG_INPUT_IN_MENU
 *             - is registered when the user keypress is not '1', '2', '3', or 'A'
 *
 *     In the Game
 *
 *         WRONG_INPUT_IN_FOOD_INPUT
 *             - is registered when the user chooses a number of food
 *               outside the range from 2 to 9
 *         WRONG_INPUT_IN_GAME
 *             - is registered during the game, when the user presses
 *               keys other than 'W', 'S', 'A', or 'D' to move Pacman
 *             - is also called during the game, when the user presses
 *               keys other than 'M' to return to menu or 'X' to
 *               exit the game
 *         WRONG_INPUT_IN_GAME_PROMPTS
 *             - is registered after the results of the game displayed
 *               through the game prompts, when the user presses
 *               keys other than 'R' to restart, 'M' to return to
 *               menu, or 'X' to exit
 *
 *     In Tutorial (WRONG_INPUT_IN_TUTORIAL)
 *
 *         WRONG_INPUT_IN_TUTORIAL
 *             - is registered when the user presses keys other than
 *               '←' or '→' to navigate through the tutorial slides
 *             - is also registered when the user presses keys other than
 *               'M' to return to menu
 *             
 *              Note that in the last tutorial slide, the player is prompted to
 *              choose again from some options. Hence, WRONG_INPUT_IN_TUTORIAL
 *              is also called when the user presses keys other than '1' to
 *              start the game, 'M' to return to menu, or 'X' to exit
 *
 *     In About the Game (WRONG_INPUT_IN_ABOUT_GAME)
 *
 *             - is registered when the user presses keys other than
 *               'M' to return to menu
 */
enum WrongInputState { 
    WRONG_INPUT_NONE,
    WRONG_INPUT_IN_MENU,
    WRONG_INPUT_IN_GAME,
    WRONG_INPUT_IN_GAME_PROMPTS,
    WRONG_INPUT_IN_ABOUT_GAME,
    WRONG_INPUT_IN_TUTORIAL,
    WRONG_INPUT_IN_FOOD_INPUT,
};

/* 
 * MenuChoiceState is associated with the menu option that the player chooses.
 * Recall that in the menu, the player can choose from the options 'Start',
 * 'Tutorial', 'About the Game', and 'Exit'.
 */
enum MenuChoiceState {
    PLAYER_CHOSE_START,
    PLAYER_CHOSE_TUTORIAL,
    PLAYER_CHOSE_ABOUT,
    PLAYER_CHOSE_EXIT,
    PLAYER_CHOSE_NONE,
};

typedef struct States {
    enum PlayerState         player_state;
    enum GameState           game_state;
    enum WrongInputState     wrong_input_state;
    enum MenuChoiceState     current_menu_choice;
    int                      current_tutorial_page;
    int                      current_number_of_foods_picked;
    bool                     show_quit_confirmation;
    bool                     player_wants_to_quit;
    Uint32                   wrong_input_time; // Uint32 is a type defined by SDL
} States;


/*
 * PacmanMove is an enum that handles the movement of pacman on the screen.
 */

enum PacmanMove {
   MOVE_PACMAN_UP,
   MOVE_PACMAN_DOWN,
   MOVE_PACMAN_LEFT,
   MOVE_PACMAN_RIGHT,
};


#endif
