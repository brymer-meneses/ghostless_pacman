#ifndef ASSETS_H
#define ASSETS_H

#include "declarations.h"

Assets load_all_assets(SDL_Renderer *renderer);
void free_all_assets(Assets *assets);

#endif // !ASSETS_H


