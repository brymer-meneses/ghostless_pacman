#include "stdio.h"

#include "utils.h"
#include "declarations.h"
#include "logic.h"
#include "assets.h"


Assets load_all_assets(SDL_Renderer *renderer) { 

    Assets assets;

    // SDL_Rect is a struct that holds
    SDL_Rect pacman_rect        = {.x=PACMAN_INITIAL_POSITION_X,  .y=PACMAN_INITIAL_POSITION_Y, .h=35, .w=35};
    SDL_Rect block_rect         = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect          = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect exit_rect          = {.x=ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=33, .w=23};
    SDL_Rect fullscreen_rect    = {.x=0,    .y=0,   .h=640, .w=640};
    SDL_Rect prompt_rect        = {.x=70,   .y=70,  .h=500, .w=500};
    SDL_Rect food_input_rect    = {.x=61,   .y=66,  .h=518, .w=507};
    SDL_Rect score_visuals_rect = {.x=450,  .y=20,  .h=39,  .w=156};
    SDL_Rect reminder_rect      = {.x=43,   .y=700, .h=98,  .w=554};
    SDL_Rect quit_rect          = {.x=36,   .y=193, .h=253, .w=567};

    // Load all sprites used for the game
    assets.game.pacman = load_sprite(renderer, "../assets/game_elements/pacman.png", 40, 10, pacman_rect);
    assets.game.main   = load_sprite(renderer, "../assets/game_elements/main.png", 0, 1, fullscreen_rect);
    assets.game.exit   = load_sprite(renderer, "../assets/game_elements/exit.png", 0, 1, exit_rect);

    // Load screen menus
    assets.misc.about_screen               = load_sprite(renderer, "../assets/misc/about_game.png", 0, 1, fullscreen_rect);

    // Load menu highlights
    assets.menu.player_chose_start         = load_sprite(renderer, "../assets/menu/highlight_start.png", 0, 1, fullscreen_rect);
    assets.menu.player_chose_tutorial      = load_sprite(renderer, "../assets/menu/highlight_tutorial.png", 0, 1, fullscreen_rect);
    assets.menu.player_chose_exit          = load_sprite(renderer, "../assets/menu/highlight_exit.png", 0, 1, fullscreen_rect);
    assets.menu.player_chose_about         = load_sprite(renderer, "../assets/menu/highlight_about.png", 0, 1, fullscreen_rect);
    assets.menu.player_chose_none          = load_sprite(renderer, "../assets/menu/highlight_none.png", 0, 1, fullscreen_rect);

    // Load game prompts
    assets.prompt.game_lost_hit_block         = load_sprite(renderer, "../assets/results/block_hit.png", 0, 1, prompt_rect);
    assets.prompt.game_lost_hit_border        = load_sprite(renderer, "../assets/results/border_hit.png", 0, 1, prompt_rect);
    assets.prompt.game_lost_insufficient_food = load_sprite(renderer, "../assets/results/food_miss.png", 0, 1, prompt_rect);
    assets.prompt.game_won                    = load_sprite(renderer, "../assets/results/won.png", 0, 1, prompt_rect);

    assets.misc.quit_confirmation_prompt =  load_sprite(renderer, "../assets/misc/quit_confirmation.png", 0, 1, quit_rect);


    char filename[35];

    // Populate the memory allocated sprite array with tutorial slides.
    for (int i = 0; i < NUMBER_OF_TUTORIAL_SLIDES; i++) {
        sprintf(filename, "../assets/tutorial/slide_%d.png", i+1);
        assets.misc.tutorial_slides[i] = load_sprite(renderer, filename, 0, 1, fullscreen_rect);
    }

    // Populate the memory allocated sprite array with food input prompts.
    for (int i = 0; i < 9; i++) {
        sprintf(filename, "../assets/food_input_prompts/%d.png", i);
        assets.misc.food_input_prompts[i] = load_sprite(renderer, filename, 0, 1, food_input_rect);
    };

    // Populate the memory allocated sprite array with score visualization
    for (int i = 0; i < 10; i++) {
        sprintf(filename, "../assets/score_visuals/%d.png", i);
        assets.misc.score_visuals[i] = load_sprite(renderer, filename, 0, 1, score_visuals_rect);
    };

    // Populate the memory allocated sprite array with blocks
    for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
        assets.game.blocks[i] = load_sprite(renderer, "../assets/game_elements/box.png", 0 , 1, block_rect);
    }

    // Populate the memory allocated sprite array with food
    for (int i=0; i<MAX_NUMBER_OF_FOOD; i++) {
        assets.game.foods[i] = load_sprite(renderer, "../assets/game_elements/food.png", 40 , 5, food_rect);
        // randomize the start of the animation by setting the current frame to different values
        assets.game.foods[i].current_frame = i;
    }

    // Load reminders
    assets.reminders.wrong_input_in_menu          = load_sprite(renderer, "../assets/reminders/wrong_input_in_menu.png", 0, 1, reminder_rect);
    assets.reminders.wrong_input_in_tutorial      = load_sprite(renderer, "../assets/reminders/wrong_input_in_tutorial.png", 0, 1, reminder_rect);
    assets.reminders.wrong_input_in_about_game    = load_sprite(renderer, "../assets/reminders/wrong_input_in_about.png", 0, 1, reminder_rect);
    assets.reminders.wrong_input_in_food_input    = load_sprite(renderer, "../assets/reminders/wrong_input_in_food_input.png", 0, 1, reminder_rect);
    assets.reminders.wrong_input_in_game          = load_sprite(renderer, "../assets/reminders/wrong_input_in_game.png", 0, 1, reminder_rect);
    assets.reminders.wrong_input_in_game_prompts  = load_sprite(renderer, "../assets/reminders/wrong_input_in_game_prompts.png", 0, 1, reminder_rect);

    // Load game sounds
    assets.sounds.background_music     = Mix_LoadMUS("../assets/sounds/background_muzic.mp3");
    assets.sounds.pacman_munch         = Mix_LoadWAV("../assets/sounds/munch.wav");
    assets.sounds.pacman_step          = Mix_LoadWAV("../assets/sounds/step.wav");
    assets.sounds.game_notification    = Mix_LoadWAV("../assets/sounds/notif.wav");
    assets.sounds.option_select        = Mix_LoadWAV("../assets/sounds/select.wav");
    assets.sounds.option_confirm       = Mix_LoadWAV("../assets/sounds/confirm.wav");
    assets.sounds.open_about_game      = Mix_LoadWAV("../assets/sounds/open_about.wav");
    assets.sounds.quit_sound           = Mix_LoadWAV("../assets/sounds/quit.wav");
    assets.sounds.start_game           = Mix_LoadWAV("../assets/sounds/start_game.wav");
    assets.sounds.game_win             = Mix_LoadWAV("../assets/sounds/win.wav");
    assets.sounds.game_over            = Mix_LoadWAV("../assets/sounds/game_over.wav");    

    return assets;
}

void free_sprite_array(Sprite* sprite_array, size_t length_of_array) {
    for (int i=0; i<length_of_array; i++) {
        SDL_DestroyTexture(sprite_array[i].texture);
    }
}


void free_all_assets(Assets *assets) { 
    // Free all sprites
    SDL_DestroyTexture(assets->game.pacman.texture);
    SDL_DestroyTexture(assets->game.exit.texture);
    SDL_DestroyTexture(assets->game.main.texture);
    free_sprite_array(assets->game.foods, MAX_NUMBER_OF_FOOD);
    free_sprite_array(assets->game.blocks, NUMBER_OF_BLOCKS);

    SDL_DestroyTexture(assets->menu.player_chose_about.texture);
    SDL_DestroyTexture(assets->menu.player_chose_start.texture);
    SDL_DestroyTexture(assets->menu.player_chose_tutorial.texture);
    SDL_DestroyTexture(assets->menu.player_chose_exit.texture);
    SDL_DestroyTexture(assets->menu.player_chose_none.texture);

    SDL_DestroyTexture(assets->prompt.game_lost_hit_block.texture);
    SDL_DestroyTexture(assets->prompt.game_lost_insufficient_food.texture);
    SDL_DestroyTexture(assets->prompt.game_won.texture);

    SDL_DestroyTexture(assets->misc.quit_confirmation_prompt.texture);

    free_sprite_array(assets->misc.tutorial_slides, NUMBER_OF_TUTORIAL_SLIDES);
    free_sprite_array(assets->misc.food_input_prompts, NUMBER_OF_FOOD_INPUT_PROMPTS);
    free_sprite_array(assets->misc.score_visuals, NUMBER_OF_SCORE_VISUALS);


    // Free all sounds
    Mix_FreeMusic(assets->sounds.background_music);
    Mix_FreeChunk(assets->sounds.pacman_munch);
    Mix_FreeChunk(assets->sounds.pacman_step);
    Mix_FreeChunk(assets->sounds.game_notification);
    Mix_FreeChunk(assets->sounds.game_win);
    Mix_FreeChunk(assets->sounds.game_over);
}