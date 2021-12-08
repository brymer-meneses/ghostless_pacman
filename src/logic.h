#ifndef LOGIC_H
#define LOGIC_H

#include "declarations.h"

void move_pacman(enum PacmanMove move, Assets *assets, Board* board, States* states);

void render_board(Board* board, Assets *assets);

void init_board(Board* board, Assets *assets, int number_of_foods);


#endif // !DECLARE_H
