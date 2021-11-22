
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#include "declarations.h"
#include "utils.h"
#include "logic.h"

typedef enum BoardElement {
    EMPTY,
    PACMAN,
    FOOD,
    BLOCK,
    EXIT,
} BoardElement;

typedef enum Move {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
} Move;


static int MAP[10][10];

// TODO: Get number of food from user
// DEBUG: Set it to 8 for now
int NUMBER_OF_FOODS = 8;
static int PLAYER_SCORE = 0;


void display_board(GameSprites* all_sprites) {

    Sprite* grid = &all_sprites->grid;
    Sprite* pacman = &all_sprites->pacman;
    Sprite* exit = &all_sprites->exit;

    Sprite* foods = all_sprites->foods;
    Sprite* blocks = all_sprites->blocks;

    draw_sprite(grid);
    draw_sprite(pacman);
    draw_sprite(exit);

    int current_food_index = 0;
    int current_block_index = 0;

    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            BoardElement element = MAP[row][col];
                switch (element) {
                    case EMPTY:
                    case PACMAN:
                        continue;
                        break;
                    case FOOD:
                        foods[current_food_index].rect.y =  ELEMENT_INITIAL_POSITION_Y + 5 +  (44 * col);
                        foods[current_food_index].rect.x =  ELEMENT_INITIAL_POSITION_X + 5 +  (44 * row);
                        draw_sprite(&foods[current_food_index]);
                        current_food_index++;
                        break;
                    case BLOCK:
                        blocks[current_block_index].rect.y =  ELEMENT_INITIAL_POSITION_Y + 5 + (44 * col);
                        blocks[current_block_index].rect.x =  ELEMENT_INITIAL_POSITION_X + 5 + (44 * row);
                        draw_sprite(&blocks[current_block_index]);
                        current_block_index++;
                        break;
                    case EXIT:
                        exit->rect.x = ELEMENT_INITIAL_POSITION_X + 5 + (44 * col);
                        exit->rect.y = ELEMENT_INITIAL_POSITION_Y + 5 + (44 * col);
                        break;
                }
        }
    }
}

Position query_pacman_position(Sprite* pacman) {
    Position pos;
    pos.x = (pacman->rect.x - ELEMENT_INITIAL_POSITION_X) / 44;
    pos.y = (pacman->rect.y - ELEMENT_INITIAL_POSITION_Y) / 44;
    return pos;
};

void init_board() {
    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            MAP[row][col] = EMPTY;
        }
    }
}

PlayerState check_player_status(Position future_position) {
    int x = future_position.x;
    int y = future_position.y;

    bool player_within_horizontal_borders = y >= 0 && y <=9;
    bool player_within_vertical_borders = x >= 0 && x <= 9;

    if (MAP[x][y] == EXIT) {
        return PLAYER_WON;
    } 
    if (MAP[x][y] == BLOCK) {
        return PLAYER_LOST_HIT_BLOCK;
    }
    if (player_within_horizontal_borders && player_within_vertical_borders) {
        return PLAYER_STILL_PLAYING;
    } else {
        return PLAYER_LOST_HIT_BORDER;
    }
    return PLAYER_LOST_INSUFFICIENT_FOOD;
};

void fill_board_with_food() {
    int total_foods_generated = 0;
    while (total_foods_generated < NUMBER_OF_FOODS){
         int rand_x = rand() % 10;
         int rand_y = rand() % 10;
         if (MAP[rand_x][rand_y] == EMPTY && MAP[rand_x][rand_y] != EXIT && rand_x > 0 && rand_y > 0) {
            MAP[rand_x][rand_y] = FOOD;
            total_foods_generated++;
        }
    }
}

void fill_board_with_blocks() {
    int total_blocks_generated = 0;
    while (total_blocks_generated < NUMBER_OF_BLOCKS) {
         int rand_x = rand() % 10;
         int rand_y = rand() % 10;
         if (MAP[rand_x][rand_y] == EMPTY && MAP[rand_x][rand_y] != EXIT && rand_x > 0 && rand_y > 0) {
             MAP[rand_x][rand_y] = BLOCK;
             total_blocks_generated++;
        }
    }
}

void move_pacman(Move move, GameSprites* all_sprites) {

    Sprite* pacman = &all_sprites->pacman;

    Position current_position = query_pacman_position(pacman);
    Position future_position;
    PlayerState status;

    switch (move) {
        case MOVE_UP:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y + -1;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;

            status = check_player_status(future_position);
            if (status == PLAYER_LOST_HIT_BORDER || PLAYER_LOST_INSUFFICIENT_FOOD || PLAYER_LOST_HIT_BLOCK) {
                puts("You lost");
                break;
            }

            pacman->rect.y -= 44;
            break;
        case MOVE_DOWN:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y +  1;

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;

            status = check_player_status(future_position);
            if (status == PLAYER_LOST_HIT_BORDER || PLAYER_LOST_INSUFFICIENT_FOOD || PLAYER_LOST_HIT_BLOCK) {
                puts("You lost");
                break;
            }

            pacman->rect.y += 44;

            break;
        case MOVE_LEFT:
            future_position.x =  current_position.x + -1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;

            status = check_player_status(future_position);
            if (status == PLAYER_LOST_HIT_BORDER || PLAYER_LOST_INSUFFICIENT_FOOD || PLAYER_LOST_HIT_BLOCK) {
                puts("You lost");
                break;
            } 

            pacman->rect.x -= 44;
            break;
        case MOVE_RIGHT:
            future_position.x =  current_position.x +  1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;

            status = check_player_status(future_position);
            if (status == PLAYER_LOST_HIT_BORDER || PLAYER_LOST_INSUFFICIENT_FOOD || PLAYER_LOST_HIT_BLOCK) {
                puts("You lost");
                break;
            } 

            pacman->rect.x += 44;
            
            break;

    }
    BoardElement future_obstacle = MAP[future_position.x][future_position.y];
    if (future_obstacle == FOOD) {
        PLAYER_SCORE++;
        printf("New Score: %d\n", PLAYER_SCORE);
        MAP[future_position.x][future_position.y] = EMPTY;
    }

    if (status == PLAYER_WON) {
        puts("You won!");
    } 
}

void handle_keypress(SDL_Event event, PlayerState player_state, GameSprites* all_sprites) {
    /* 
        A function that handles the various keypresses of the user.
        params: 
            SDL_Event event 
                - Specifies an `event` in the game like when the player presses the button `w` on their keyboard.
            Game *game
                - Pointer to an instance of the struct `Game`.
        returns: 
            NONE
            
    */
    bool is_game_running = player_state == PLAYER_STILL_PLAYING;
    if (is_game_running) {

        switch (event.key.keysym.scancode) {
            // Moving pacman
            case SDL_SCANCODE_W:
                move_pacman(MOVE_UP, all_sprites);
                break;
            case SDL_SCANCODE_A:
                move_pacman(MOVE_LEFT, all_sprites);
                break;
            case SDL_SCANCODE_S:
                move_pacman(MOVE_LEFT, all_sprites);
                break;
            case SDL_SCANCODE_D:
                move_pacman(MOVE_LEFT, all_sprites);
                break;
            default:
                // TODO: Warn the user if the input is invalid
                // perhaps add some instructions?
                break;
            }
    } else {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                if (is_game_running) move_pacman(MOVE_UP, all_sprites);
                break;
            default:
                // TODO: Warn the user if the input is invalid
                // perhaps add some instructions?
                break;
        }
    }

    }
}

GameSprites load_all_sprites(SDL_Renderer* renderer) {
    SDL_Rect pacman_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "./assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {GRID_POSITION_X, GRID_POSITION_Y, GRID_SIZE, GRID_SIZE};
    Sprite grid = load_sprite(renderer, "./assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {GRID_POSITION_X, GRID_POSITION_Y, 23, 33};
    Sprite exit = load_sprite(renderer, "./assets/exit.png", 0, 1, exit_rect);

    SDL_Rect home_screen_rect = {0, 0, 640, 640};
    Sprite home_screen = load_sprite(renderer, "./assets/homescreen.png", 0, 1, home_screen_rect);

    SDL_Rect block_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SDL_Rect food_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=27, .w=23};

    GameSprites sprites;
    sprites.pacman = pacman;
    sprites.grid = grid;
    sprites.exit = exit;
    sprites.home_screen = home_screen;

    sprites.blocks = (Sprite*) malloc(NUMBER_OF_BLOCKS * sizeof(Sprite));
    sprites.foods = (Sprite*) malloc(NUMBER_OF_FOODS * sizeof(Sprite));

    for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
        sprites.blocks[i] = load_sprite(renderer, "./assets/box.png", 0 , 1, block_rect);
    }

    for (int i=0; i<NUMBER_OF_FOODS; i++) {
        sprites.foods[i] = load_sprite(renderer, "./assets/food.png", 0 , 1, food_rect);
    }


    return sprites;
}


void init_game() {

    time_t t;
    srand((unsigned) time(&t));

    init_board();

    int exit_x = 0;
    int exit_y = 0;
    while (exit_x == 0 || exit_y == 0) {
        exit_x = rand() % 10;
        exit_y = rand() % 10;
    }
    MAP[exit_x][exit_y] = EXIT;

    fill_board_with_blocks();
    fill_board_with_food();
}

void handle_state(PlayerState state, GameSprites* all_sprites) {
    Sprite home_screen = all_sprites->home_screen;
    switch (state) {
        case PLAYER_ON_MENU:
            draw_sprite(&home_screen);
            break;
        case PLAYER_ON_TUTORIAL:
            break;
        case PLAYER_ON_ABOUT:
            break;
        case PLAYER_STILL_PLAYING:
            display_board(all_sprites);
            break;
        case PLAYER_WON:
            break;
        case PLAYER_LOST_HIT_BLOCK:
            break;
        case PLAYER_LOST_HIT_BORDER:
            break;
        case PLAYER_LOST_INSUFFICIENT_FOOD:
            break;
    }
}
