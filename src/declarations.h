#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define WINDOW_HEIGHT 640
#define WINDOW_WIDTH 640

// Ensure that the grid is centered
#define GRID_SIZE 450

#define GRID_POSITION_X ((WINDOW_WIDTH - GRID_SIZE)/2)
#define GRID_POSITION_Y ((WINDOW_HEIGHT - GRID_SIZE)/2)

#define SPRITE_UPDATE_DELAY 100

#define NUMBER_OF_BLOCKS 10
#define NUMBER_OF_TUTORIAL_SLIDES 7

#define ELEMENT_INITIAL_POSITION_X (GRID_POSITION_X + 8)
#define ELEMENT_INITIAL_POSITION_Y (GRID_POSITION_Y + 8)

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
        Sprite grid;
        Sprite exit;
        Sprite* blocks;
        Sprite* foods ;
    } game;


    struct { 
        Sprite* tutorial_slides;
        Sprite* food_input_prompts;
        Sprite homescreen;
        Sprite about_screen;
    } misc;

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

typedef struct States {
    enum PlayerState         player_state;
    enum GameState           game_state;
    enum WrongKeyState       wrong_key_state;
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
