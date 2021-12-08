#ifndef STATE_H
#define STATE_H

#include "utils.h"
#include "logic.h"

void render_state(States *states, Board* board, Assets *assets);
void process_keypress(SDL_Event event, States *states, Board* board, Assets *assets);

#endif
