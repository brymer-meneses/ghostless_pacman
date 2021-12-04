#ifndef LOGIC_H
#define LOGIC_H

#include "declarations.h"

void move_pacman(Move move, Assets *assets, Map* map);

Map* init_map();
void render_map(Map* map, Assets *assets);


#endif // !DECLARE_H
