#include "utils.h"
#include "logic.h"

void handle_state(PlayerState state, Map* map, GameSprites* game_sprites, MiscSprites* misc_sprites, PromptSprites* prompt_sprites);
void handle_keypress(SDL_Event event, PlayerState *player_state, Map* map, GameSprites* game_sprites);

GameSprites load_all_game_sprites(SDL_Renderer* renderer, Map* map);
MiscSprites load_all_misc_sprites(SDL_Renderer* renderer);
PromptSprites load_all_prompt_sprites(SDL_Renderer* renderer);
