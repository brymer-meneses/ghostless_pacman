// Standard Library Imports
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

// Project-wide Headers
#include "utils.h"
#include "logic.h"
#include "declarations.h"

int main (int argc, char *argv[]) {
    SDL_Window* window = create_window();
    SDL_Renderer* renderer = create_renderer(window);

    IMG_Init(IMG_INIT_PNG);
    SDL_SetRenderDrawColor(renderer, 236, 239, 244, 1);
     
    init_game();
    GameSprites all_sprites = load_all_sprites(renderer);
     

    bool user_wants_to_quit = false;
    while (!user_wants_to_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) user_wants_to_quit = true; 
            else if (event.type == SDL_KEYDOWN) handle_keypress(event, &all_sprites);
        }
        SDL_RenderClear(renderer);
        display_board(&all_sprites);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60); }

    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
