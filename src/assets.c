#include "utils.h"
#include "declarations.h"
#include "logic.h"
#include "assets.h"


Assets *load_all_assets(SDL_Renderer *renderer) { 


    Assets *assets = malloc(1 * sizeof(Assets));
    if (!assets) {
        puts("Memory Allocation for Assets failed");
    }


    // Load all sprites used for the game

    SDL_Rect pacman_rect = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "../assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {.x=GRID_POSITION_X, .y=GRID_POSITION_Y, .h=GRID_SIZE, .w=GRID_SIZE};
    Sprite grid = load_sprite(renderer, "../assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {.x=GRID_POSITION_X, .y=GRID_POSITION_Y, .h=33, .w=23};
    Sprite exit = load_sprite(renderer, "../assets/exit.png", 0, 1, exit_rect);


    SDL_Rect block_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};


    assets->game.pacman = pacman;
    assets->game.grid = grid;
    assets->game.exit = exit;

    assets->game.blocks = (Sprite*) malloc(NUMBER_OF_BLOCKS * sizeof(Sprite));
    assets->game.foods = (Sprite*) malloc(MAX_NUMBER_OF_FOOD * sizeof(Sprite));

    for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
        assets->game.blocks[i] = load_sprite(renderer, "../assets/box.png", 0 , 1, block_rect);
    }

    for (int i=0; i<MAX_NUMBER_OF_FOOD; i++) {
        assets->game.foods[i] = load_sprite(renderer, "../assets/food.png", 40 , 5, food_rect);
        // randomize the start of the animation by setting the current frame to different values
        assets->game.foods[i].current_frame = i;
    }


    SDL_Rect screen_rect = {0, 0, 640, 640};
    SDL_Rect prompt_rect = {108, 104, 423, 430};

    // Load screen displays

    Sprite homescreen = load_sprite(renderer, "../assets/homescreen.png", 0, 1, screen_rect);
    Sprite about_screen = load_sprite(renderer, "../assets/about.png", 0, 1, screen_rect);
    Sprite game_lost_hit_block = load_sprite(renderer, "../assets/overBlock.png", 0, 1, prompt_rect);
    Sprite game_lost_hit_border = load_sprite(renderer, "../assets/overOut.png", 0, 1, prompt_rect);
    Sprite game_lost_insufficient_food = load_sprite(renderer, "../assets/overMiss.png", 0, 1, prompt_rect);
    Sprite game_won = load_sprite(renderer, "../assets/win.png", 0, 1, prompt_rect);

    assets->misc.homescreen = homescreen;
    assets->misc.about_screen = about_screen;
    assets->prompt.game_lost_hit_block = game_lost_hit_block;
    assets->prompt.game_lost_hit_border = game_lost_hit_border;
    assets->prompt.game_lost_insufficient_food = game_lost_insufficient_food;
    assets->prompt.game_won = game_won;

    assets->misc.tutorial_slides = (Sprite *) malloc(NUMBER_OF_TUTORIAL_SLIDES * sizeof(Sprite));

    // Load tutorial slides
    char filename[31];
    for (int i=0; i<NUMBER_OF_TUTORIAL_SLIDES; i++) {
        sprintf(filename, "../assets/tutorial/slide_%d.png", i+1);
        assets->misc.tutorial_slides[i] = load_sprite(renderer, filename, 0, 1, screen_rect);
    }


    // Load game sounds
    assets->sounds.background_music = Mix_LoadMUS("../assets/sounds/background_muzic.mp3");
    assets->sounds.pacman_munch = Mix_LoadWAV("../assets/sounds/munch.wav");
    assets->sounds.pacman_step = Mix_LoadWAV("../assets/sounds/step.wav");
    assets->sounds.game_notification = Mix_LoadWAV("../assets/sounds/notif.wav");
    assets->sounds.game_win = Mix_LoadWAV("../assets/sounds/win.wav");
    assets->sounds.game_over = Mix_LoadWAV("../assets/sounds/game_over.wav");    

    return assets;
}


void free_all_assets(Assets *assets) { 
    // Free all sprites
    SDL_DestroyTexture(assets->game.pacman.texture);
    SDL_DestroyTexture(assets->game.blocks->texture);
    SDL_DestroyTexture(assets->game.foods->texture);
    SDL_DestroyTexture(assets->game.exit.texture);
    SDL_DestroyTexture(assets->game.grid.texture);
    SDL_DestroyTexture(assets->misc.homescreen.texture);

    // Free all sounds
    Mix_FreeMusic(assets->sounds.background_music);
    Mix_FreeChunk(assets->sounds.pacman_munch);
    Mix_FreeChunk(assets->sounds.pacman_step);
    Mix_FreeChunk(assets->sounds.game_notification);
    Mix_FreeChunk(assets->sounds.game_win);
    Mix_FreeChunk(assets->sounds.game_over);
    Mix_CloseAudio();
}


