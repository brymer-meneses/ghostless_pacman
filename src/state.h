#include "utils.h"
#include "logic.h"

void handle_state(PlayerState state, Map* map, Assets *assets);
void handle_keypress(SDL_Event event, PlayerState *player_state, Map* map, Assets *assets, int *current_tutorial_slide_number);

