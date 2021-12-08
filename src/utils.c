/*
 *  This file, `utils.c`, contains utility functions that are used throughout
 *  the program. 
 */


#include "stdbool.h"
#include "stdio.h"

#include "utils.h"
#include "declarations.h"

#include "SDL2/SDL_mixer.h"

void init_SDL() {
    // Turn on anti-aliasing by default
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    // Tell SDL to initialize loading of png images. 
    IMG_Init(IMG_INIT_PNG);

    // Initializes SDL, if it fails exit with an error code.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        SDL_Quit();
    }  

    // Initialize SDL audio system and mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT , 2, 2048);
}


SDL_Window* create_window() {
    // A function that initializes SDL and opens up a window


    // Initialize a window using SDL
    SDL_Window* window = SDL_CreateWindow("Ghostless Pacman",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // If the window initialization fails, quit sdl
    if (!window) { 
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
    }
    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window) {
    /*
     *  A function that creates a renderer, which manages the graphics hardware. 
     *  A `renderer` is a variable defined in SDL that handles the rendering
     *  of any element in the screen.
     */

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return renderer;
}

void render_sprite(Sprite *sprite) {

    bool is_sprite_static = sprite->total_frames == 1;

    if (is_sprite_static) {
        SDL_RenderCopy(sprite->renderer, sprite->texture, NULL, &sprite->rect);
    } else {
        Uint32 ticks = SDL_GetTicks();
        Uint32 current_frame = ( ticks/ SPRITE_UPDATE_DELAY  + sprite->current_frame) % sprite->total_frames;
        SDL_Rect clip_rect = { current_frame * sprite->frame_size, 0, sprite->rect.w, sprite->rect.h };
        SDL_RenderCopyEx(sprite->renderer, sprite->texture, &clip_rect, &sprite->rect, sprite->rotation, NULL, sprite->flip);
    }
}

Sprite load_sprite(SDL_Renderer *renderer, char* filename, int sprite_frame_size, int total_frames, SDL_Rect rect) {
    /*
     *
     */

    SDL_Surface* image_surface = IMG_Load(filename);
    if (!image_surface) {
        printf("Error creating surface\n");
    }
    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    if (!image_texture) {
        printf("Error creating texture: %s\n", SDL_GetError());

    }
    SDL_FreeSurface(image_surface);


    Sprite sprite;
    sprite.renderer = renderer;
    sprite.frame_size = sprite_frame_size;
    sprite.total_frames = total_frames;
    sprite.texture = image_texture;
    sprite.current_frame = 0;
    sprite.rect = rect;
    return sprite;
}

int gen_random_num(int min, int max) {
    /* 
     * Generates a random number with a minimum or maximum.
     * 
     * params:
     *      int max
     *          the maximum number that will be generated
     *      int min
     *          the minimum number that will be generated
     * returns:
     *      a random number generated in with range [min, max].
     */

    return rand() % (max + 1 - min) + min;
}


void render_reminder(Sprite* sprite, States* states, Uint32 duration, int maximum_height, int climb_speed){
    render_sprite(sprite);
    Uint32 time_until_reminder_goes_down = states->wrong_input_time + duration;

    if (time_until_reminder_goes_down < SDL_GetTicks()) {
        sprite->rect.y += climb_speed;
    } else {
        if (sprite->rect.y >= maximum_height) {
            sprite->rect.y -= climb_speed;
        }
    }
}
