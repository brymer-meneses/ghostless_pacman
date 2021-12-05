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
        Sprite homescreen;
        Sprite about_screen;
    } misc;

    struct { 
        Sprite player_won;
        Sprite player_lost_hit_block;
        Sprite player_lost_hit_border;
        Sprite player_lost_insufficient_food;
        Sprite wrong_key_during_game;
        Sprite wrong_key_during_menu;
        Sprite wrong_key_during_instructions;
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


typedef enum PlayerState {
    PLAYER_WON,
    PLAYER_LOST_HIT_BLOCK,
    PLAYER_LOST_HIT_BORDER,
    PLAYER_LOST_INSUFFICIENT_FOOD,

    PLAYER_IN_GAME,
    PLAYER_IN_MENU,
    PLAYER_IN_TUTORIAL,
    PLAYER_IN_ABOUT,
} PlayerState;

typedef struct States {
    PlayerState player_state;
    int current_tutorial_slide_index;
} States;

typedef enum BoardElement {
    EMPTY,
    PACMAN,
    FOOD,
    BLOCK,
    EXIT,
} BoardElement;

typedef enum Move {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
} Move;



#endif
