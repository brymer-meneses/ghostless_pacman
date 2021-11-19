#ifndef LOGIC_H
#define LOGIC_H
#include "utils.h"

typedef struct GameSprites {
    Sprite pacman;
    Sprite grid;
    Sprite exit;
    Sprite *blocks;
    Sprite *foods ;
} GameSprites;

void display_board(GameSprites* all_sprites);
GameSprites load_all_sprites(SDL_Renderer* renderer);

void init_game();

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

extern int NUMBER_OF_FOODS;

void handle_state(PlayerState state, GameSprites* all_sprites);
void handle_keypress(SDL_Event event, PlayerState player_state, GameSprites* all_sprites);
#endif // !DECLARE_H
