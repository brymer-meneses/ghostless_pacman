#ifndef LOGIC_H
#define LOGIC_H

#include "declarations.h"

void move_pacman(enum Move move, Assets *assets, Map* map, States* states);

void render_map(Map* map, Assets *assets);

void init_map(Map* map, Assets *assets, int number_of_foods);


#endif // !DECLARE_H
