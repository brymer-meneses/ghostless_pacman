#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
    #include "lib/SDL2/SDL.h"
#else
    #include "SDL2/SDL.h"
    #include "SDL2/SDL_image.h"
#endif

typedef struct Sprite {
    SDL_Renderer        *renderer;
    SDL_Texture         *texture;
    SDL_Rect            rect;
    SDL_RendererFlip    flip;
    float               rotation;
    int                 total_frames;
    int                 frame_size;
} Sprite;

typedef struct SpriteCollection {
    SDL_Renderer        *renderer;
    SDL_Texture         *texture;
    SDL_Rect            *rect;
    SDL_RendererFlip    flip;
    float               rotation;
    int                 total_frames;
    int                 frame_size;
    int                 total_sprites;
} SpriteCollection;

typedef struct Position {
    int x;
    int y;
} Position;



SDL_Renderer* create_renderer(SDL_Window* window);
SDL_Window* create_window();
Sprite load_sprite(SDL_Renderer *renderer, char* filename, int sprite_frame_size, int total_frames, SDL_Rect rect);
void draw_sprite(Sprite *sprite);

SpriteCollection load_sprite_collection(int number_of_sprites, 
                                        SDL_Renderer* renderer,
                                        char* filename,
                                        int sprite_frame_size,
                                        int total_frames, 
                                        SDL_Rect rect);
void draw_sprite_collection(SpriteCollection *sprite_collection);
#endif // !UTILS_H
