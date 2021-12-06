#include "stdio.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"
#include "assets.h"


Assets *load_all_assets(SDL_Renderer *renderer) { 

    // Memory allocation for a single asset
    Assets *assets = malloc(1 * sizeof(Assets));
    if (!assets) {
        puts("Memory Allocation for Assets failed");
    }

    // SDL_Rect is a struct that holds
    SDL_Rect pacman_rect        = {.x=PACMAN_INITIAL_POSITION_X,  .y=PACMAN_INITIAL_POSITION_Y, .h=35, .w=35};
    SDL_Rect block_rect         = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect          = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect exit_rect          = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=33, .w=23};
    SDL_Rect fullscreen_rect    = {.x=0,   .y=0,   .h=640, .w=640};
    SDL_Rect prompt_rect        = {.x=108, .y=104, .h=423, .w=430};
    SDL_Rect food_input_rect    = {.x=61,  .y=66,  .h=518, .w=507};
    SDL_Rect score_visuals_rect = {.x=450,  .y=20,  .h=39, .w=156};

    // Load all sprites used for the game
    assets->game.pacman = load_sprite(renderer, "../assets/game_elements/pacman.png", 40, 10, pacman_rect);
    assets->game.main   = load_sprite(renderer, "../assets/game_elements/main.png", 0, 1, fullscreen_rect);
    assets->game.exit   = load_sprite(renderer, "../assets/game_elements/exit.png", 0, 1, exit_rect);

    // Load screen menus
    assets->misc.homescreen                    = load_sprite(renderer, "../assets/misc/homescreen.png", 0, 1, fullscreen_rect);
    assets->misc.about_screen                  = load_sprite(renderer, "../assets/misc/about_game.png", 0, 1, fullscreen_rect);

    // Load game prompts
    assets->prompt.game_lost_hit_block         = load_sprite(renderer, "../assets/results/block_hit.png", 0, 1, prompt_rect);
    assets->prompt.game_lost_hit_border        = load_sprite(renderer, "../assets/results/border_hit.png", 0, 1, prompt_rect);
    assets->prompt.game_lost_insufficient_food = load_sprite(renderer, "../assets/results/food_miss.png", 0, 1, prompt_rect);
    assets->prompt.game_won                    = load_sprite(renderer, "../assets/results/won.png", 0, 1, prompt_rect);

    // Memory allocation for an array containing sprites
    assets->misc.tutorial_slides    = (Sprite *) malloc(NUMBER_OF_TUTORIAL_SLIDES * sizeof(Sprite));
    assets->misc.food_input_prompts = (Sprite *) malloc(9 * sizeof(Sprite));
    assets->misc.score_visuals      = (Sprite *) malloc(10 * sizeof(Sprite));
    assets->game.blocks             = (Sprite*) malloc(NUMBER_OF_BLOCKS * sizeof(Sprite));
    assets->game.foods              = (Sprite*) malloc(MAX_NUMBER_OF_FOOD * sizeof(Sprite));

    char tutorial_filename[31];
    char food_input_filename[35];
    char score_visuals_filename[30];

    // Populate the memory allocated sprite array with tutorial slides.
    for (int i = 0; i < NUMBER_OF_TUTORIAL_SLIDES; i++) {
        sprintf(tutorial_filename, "../assets/tutorial/slide_%d.png", i+1);
        assets->misc.tutorial_slides[i] = load_sprite(renderer, tutorial_filename, 0, 1, fullscreen_rect);
    }

    // Populate the memory allocated sprite array with food input prompts.
    for (int i = 0; i < 9; i++) {
        sprintf(food_input_filename, "../assets/food_input_prompts/%d.png", i);
        assets->misc.food_input_prompts[i] = load_sprite(renderer, food_input_filename, 0, 1, food_input_rect);
    };

    // Populate the memory allocated sprite array with score visualization
    for (int i = 0; i < 10; i++) {
        sprintf(score_visuals_filename, "../assets/score_visuals/%d.png", i);
        assets->misc.score_visuals[i] = load_sprite(renderer, score_visuals_filename, 0, 1, score_visuals_rect);
    };

    // Populate the memory allocated sprite array with blocks
    for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
        assets->game.blocks[i] = load_sprite(renderer, "../assets/game_elements/box.png", 0 , 1, block_rect);
    }

    // Populate the memory allocated sprite array with food
    for (int i=0; i<MAX_NUMBER_OF_FOOD; i++) {
        assets->game.foods[i] = load_sprite(renderer, "../assets/game_elements/food.png", 40 , 5, food_rect);
        // randomize the start of the animation by setting the current frame to different values
        assets->game.foods[i].current_frame = i;
    }

    // Load game sounds
    assets->sounds.background_music     = Mix_LoadMUS("../assets/sounds/background_muzic.mp3");
    assets->sounds.pacman_munch         = Mix_LoadWAV("../assets/sounds/munch.wav");
    assets->sounds.pacman_step          = Mix_LoadWAV("../assets/sounds/step.wav");
    assets->sounds.game_notification    = Mix_LoadWAV("../assets/sounds/notif.wav");
    assets->sounds.game_win             = Mix_LoadWAV("../assets/sounds/win.wav");
    assets->sounds.game_over            = Mix_LoadWAV("../assets/sounds/game_over.wav");    

    return assets;
}


void free_all_assets(Assets *assets) { 
    // Free all sprites
    SDL_DestroyTexture(assets->game.pacman.texture);
    SDL_DestroyTexture(assets->game.blocks->texture);
    SDL_DestroyTexture(assets->game.foods->texture);
    SDL_DestroyTexture(assets->game.exit.texture);
    SDL_DestroyTexture(assets->game.main.texture);
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


