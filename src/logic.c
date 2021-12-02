
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#include "declarations.h"
#include "utils.h"
#include "logic.h"


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

PlayerState check_if_player_won(Position future_position) {
    return PLAYER_LOST_INSUFFICIENT_FOOD;
}

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

    bool did_player_lost = false;

    switch (move) {
        case MOVE_UP:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y + -1;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;

            status = check_player_status(future_position);
            switch (status) {
                case PLAYER_LOST_HIT_BLOCK:
                case PLAYER_LOST_HIT_BORDER:
                case PLAYER_LOST_INSUFFICIENT_FOOD:
                    did_player_lost = true;
                    break;
                default:
                    break;
            }

            if (did_player_lost) break;

            pacman->rect.y -= 44;
            break;
        case MOVE_DOWN:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y +  1;

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;

            status = check_player_status(future_position);
            switch (status) {
                case PLAYER_LOST_HIT_BLOCK:
                case PLAYER_LOST_HIT_BORDER:
                case PLAYER_LOST_INSUFFICIENT_FOOD:
                    did_player_lost = true;
                    break;
                default:
                    break;
            }

            if (did_player_lost) break;
            pacman->rect.y += 44;

            break;
        case MOVE_LEFT:
            future_position.x =  current_position.x + -1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;

            status = check_player_status(future_position);
            switch (status) {
                case PLAYER_LOST_HIT_BLOCK:
                case PLAYER_LOST_HIT_BORDER:
                case PLAYER_LOST_INSUFFICIENT_FOOD:
                    did_player_lost = true;
                    break;
                default:
                    break;
            }

            if (did_player_lost) break;
            pacman->rect.x -= 44;
            break;
        case MOVE_RIGHT:
            future_position.x =  current_position.x +  1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;

            status = check_player_status(future_position);
            switch (status) {
                case PLAYER_LOST_HIT_BLOCK:
                case PLAYER_LOST_HIT_BORDER:
                case PLAYER_LOST_INSUFFICIENT_FOOD:
                    did_player_lost = true;
                    break;
                default:
                    break;
            }

            if (did_player_lost) break;
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

