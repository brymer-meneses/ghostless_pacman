#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 640

#define SPRITE_UPDATE_DELAY 100

#define NUMBER_OF_BLOCKS 10
#define NUMBER_OF_TUTORIAL_SLIDES 7

#define ELEMENT_INITIAL_POSITION_X (95 + 20)
#define ELEMENT_INITIAL_POSITION_Y (95 + 10)

#define PACMAN_INITIAL_POSITION_X (ELEMENT_INITIAL_POSITION_X - 8)
#define PACMAN_INITIAL_POSITION_Y (ELEMENT_INITIAL_POSITION_Y - 5)

#define MAX_NUMBER_OF_FOOD 9

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"

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

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Assets {

    struct {
        Sprite pacman;
        Sprite main;
        Sprite exit;
        Sprite blocks[10];
        Sprite foods[9];
    } game;


    struct { 
        Sprite tutorial_slides[7];
        Sprite food_input_prompts[9];
        Sprite score_visuals[10];
        Sprite homescreen;
        Sprite about_screen;
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
        Sprite game_in_food_input;
        Sprite game_lost_hit_block;
        Sprite game_lost_hit_border;
        Sprite game_lost_insufficient_food;
        Sprite wrong_key_in_game;
        Sprite wrong_key_in_menu;
        Sprite wrong_key_in_about_game;
        Sprite wrong_key_in_food_input;
    } prompt;

    // Struct for sounds

    struct { 
        Mix_Music* background_music;
        Mix_Chunk* pacman_munch;
        Mix_Chunk* pacman_step;
        Mix_Chunk* game_notification;
        Mix_Chunk* game_win;
        Mix_Chunk* game_over;
    } sounds;


} Assets;

typedef struct Map { 
    // Pointer to a 10 by 10, two-dimensional array
    int board[10][10];
    // Total number of foods generated in the map
    int number_of_foods;
    // Total number of blocks generated in the map
    int number_of_blocks;
    
    int total_player_score;
} Map;


enum PlayerState {
    PLAYER_IN_GAME,
    PLAYER_IN_MENU,
    PLAYER_IN_TUTORIAL,
    PLAYER_IN_ABOUT,
};

enum GameState { 
    GAME_WON,
    GAME_LOST_HIT_BLOCK,
    GAME_LOST_HIT_BORDER,
    GAME_LOST_INSUFFICIENT_FOOD,
    GAME_IN_PROGRESS,
    GAME_IN_FOOD_NUMBER_INPUT,
};

enum WrongKeyState { 
    WRONG_KEY_NONE,
    WRONG_KEY_IN_MENU,
    WRONG_KEY_IN_GAME,
    WRONG_KEY_IN_GAME_PROMPTS,
    WRONG_KEY_IN_ABOUT_GAME,
    WRONG_KEY_IN_TUTORIAL,
    WRONG_KEY_IN_FOOD_INPUT,
};

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
    enum WrongKeyState       wrong_key_state;
    enum MenuChoiceState     current_menu_choice;
    int                      current_tutorial_slide_index;
    int                      current_number_of_foods_picked;
} States;

enum BoardElement {
    EMPTY,
    PACMAN,
    FOOD,
    BLOCK,
    EXIT,
};

enum Move {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
};



#endif
