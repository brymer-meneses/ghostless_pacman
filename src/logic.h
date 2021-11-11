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

void handle_keypress(SDL_Event event, GameSprites* all_sprites);
void display_board(GameSprites* all_sprites);
GameSprites load_all_sprites(SDL_Renderer* renderer);

void init_game();

extern int NUMBER_OF_FOODS;
#endif // !DECLARE_H
