#ifndef LOGIC_H
#define LOGIC_H
#include "utils.h"

// GameSprites is a struct that contains all the 
// sprites for running the main game. 
typedef struct GameSprites {
    Sprite pacman;
    Sprite grid;
    Sprite exit;
    Sprite *blocks;
    Sprite *foods ;
} GameSprites;

typedef struct MiscSprites {
    Sprite slide_1;
    Sprite slide_2;
    Sprite slide_3;
    Sprite slide_4;
    Sprite homescreen;
} MiscSprites;

typedef struct PromptSprites { 
    Sprite player_won;
    Sprite player_lost_hit_block;
    Sprite player_lost_hit_border;
    Sprite player_lost_insufficient_food;
    Sprite wrong_key_during_game;
    Sprite wrong_key_during_menu;
    Sprite wrong_key_during_instructions;
} PromptSprites;


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
    PLAYER_STILL_PLAYING,

    PLAYER_ON_MENU,
    PLAYER_ON_TUTORIAL,
    PLAYER_ON_ABOUT,
} PlayerState;

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

extern int NUMBER_OF_FOODS;

void move_pacman(Move move, GameSprites* all_sprites, Map* map);

Map* init_map();
void render_map(Map* map, GameSprites* game_sprites);

GameSprites load_all_game_sprites(SDL_Renderer* renderer, Map* map);
MiscSprites load_all_misc_sprites(SDL_Renderer* renderer);
PromptSprites load_all_prompt_sprites(SDL_Renderer* renderer);

#endif // !DECLARE_H
