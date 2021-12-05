#ifndef LOGIC_H
#define LOGIC_H

#include "declarations.h"

void move_pacman(enum Move move, Assets *assets, Map* map, States* states);

Map* init_map();
void render_map(Map* map, Assets *assets);

void reset_map(Map* map, Assets *assets);


#endif // !DECLARE_H
