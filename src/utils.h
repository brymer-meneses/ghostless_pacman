#ifndef UTILS_H
#define UTILS_H

#include "declarations.h"

void init_SDL();

SDL_Renderer* create_renderer(SDL_Window* window);
SDL_Window* create_window();
Sprite load_sprite(SDL_Renderer *renderer, char* filename, int sprite_frame_size, int total_frames, SDL_Rect rect);
void render_sprite(Sprite *sprite);

int gen_random_num(int min, int max);
void show_notification(Sprite *sprite);
void close_notification(Sprite *sprite);

#endif // !UTILS_H
