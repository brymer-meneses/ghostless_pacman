#ifndef STATE_H
#define STATE_H

#include "utils.h"
#include "logic.h"

void render_state(States *states, Map* map, Assets *assets);
void process_keypress(SDL_Event event, States *states, Map* map, Assets *assets);

#endif
