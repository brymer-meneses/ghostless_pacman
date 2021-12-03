
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#include "declarations.h"
#include "utils.h"
#include "logic.h"

void render_map(Map *map, GameSprites *game_sprites) {

    Sprite* grid = &game_sprites->grid;
    Sprite* pacman = &game_sprites->pacman;
    Sprite* exit = &game_sprites->exit;

    Sprite* foods = game_sprites->foods;
    Sprite* blocks = game_sprites->blocks;

    render_sprite(grid);
    render_sprite(pacman);
    render_sprite(exit);

    int current_food_index = 0;
    int current_block_index = 0;

    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            BoardElement element = map->board[row][col];
                switch (element) {
                    case EMPTY:
                    case PACMAN:
                        continue;
                        break;
                    case FOOD:
                        foods[current_food_index].rect.y =  ELEMENT_INITIAL_POSITION_Y + 5 +  (44 * col);
                        foods[current_food_index].rect.x =  ELEMENT_INITIAL_POSITION_X + 5 +  (44 * row);
                        render_sprite(&foods[current_food_index]);
                        current_food_index++;
                        break;
                    case BLOCK:
                        blocks[current_block_index].rect.y =  ELEMENT_INITIAL_POSITION_Y + 5 + (44 * col);
                        blocks[current_block_index].rect.x =  ELEMENT_INITIAL_POSITION_X + 5 + (44 * row);
                        render_sprite(&blocks[current_block_index]);
                        current_block_index++;
                        break;
                    case EXIT:
                        exit->rect.y = ELEMENT_INITIAL_POSITION_Y + 5 + (44 * col);
                        exit->rect.x = ELEMENT_INITIAL_POSITION_X + 5 + (44 * row);
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


PlayerState check_player_status(Position future_position, BoardElement future_obstacle, Map *map) {
    int x = future_position.x;
    int y = future_position.y;

    bool player_within_horizontal_borders = y >= 0 && y <=9;
    bool player_within_vertical_borders = x >= 0 && x <= 9;
    bool player_within_borders = player_within_horizontal_borders && player_within_vertical_borders;

    switch (future_obstacle) {
        case FOOD:
            map->total_player_score++;
            map->board[x][y] = EMPTY;
            break;
        case BLOCK:
            return PLAYER_LOST_HIT_BLOCK;
            break;
        default: 
            break;
    }

    if (!player_within_borders) {
        return PLAYER_LOST_HIT_BORDER;
    } 

    return PLAYER_STILL_PLAYING;
};

PlayerState check_if_player_won(Position future_position, Map* map) {
    if (map->total_player_score == map->number_of_foods) {
        return PLAYER_WON;
    } else {
        return PLAYER_LOST_INSUFFICIENT_FOOD;
    }

}

void fill_board_with_food(Map *map) {
    int total_foods_generated = 0;
    while (total_foods_generated < map->number_of_foods){
         int rand_x =  rand() % 10;
         int rand_y =  rand() % 10;
         if (map->board[rand_x][rand_y] == EMPTY && rand_x != 0 && rand_y != 0) {
            map->board[rand_x][rand_y] = FOOD;
            total_foods_generated++;
        }
    }
}

void fill_board_with_blocks(Map *map) {
    int total_blocks_generated = 0;
    while (total_blocks_generated < map->number_of_blocks) {
         int rand_x =  rand() % 10;
         int rand_y =  rand() % 10;
         if (map->board[rand_x][rand_y] == EMPTY && rand_x != 0 && rand_y != 0) {
             map->board[rand_x][rand_y] = BLOCK;
             total_blocks_generated++;
        }
    }
}

void move_pacman(Move move, GameSprites* all_sprites, Map* map) {

    Sprite* pacman = &all_sprites->pacman;

    Position current_position = query_pacman_position(pacman);
    Position future_position;
    PlayerState player_state;

    BoardElement future_obstacle;
    switch (move) {
        case MOVE_UP:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y + -1;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                player_state = check_if_player_won(future_position, map);
                if (player_state == PLAYER_WON) { 
                    // display player won prompt
                    puts("You won!");
                } else {
                    // display player lost prompt
                    puts("You lost!");
                }
                break;
            } else { 
                player_state = check_player_status(future_position, future_obstacle, map);
                if (player_state != PLAYER_STILL_PLAYING) break;
            }


            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;

            pacman->rect.y -= 44;
            break;
        case MOVE_DOWN:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y +  1;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                player_state = check_if_player_won(future_position, map);
                if (player_state == PLAYER_WON) { 
                    // display player won prompt
                    puts("You won!");
                } else {
                    // display player lost prompt
                    puts("You lost!");
                }
                break;
            } else { 
                player_state = check_player_status(future_position, future_obstacle, map);
                if (player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;

            pacman->rect.y += 44;

            break;
        case MOVE_LEFT:
            future_position.x =  current_position.x + -1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                player_state = check_if_player_won(future_position, map);
                if (player_state == PLAYER_WON) { 
                    // display player won prompt
                    puts("You won!");
                } else {
                    // display player lost prompt
                    puts("You lost!");
                }
                break;
            } else { 
                player_state = check_player_status(future_position, future_obstacle, map);
                if (player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->rect.x -= 44;
            break;
        case MOVE_RIGHT:
            future_position.x =  current_position.x +  1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                player_state = check_if_player_won(future_position, map);
                if (player_state == PLAYER_WON) { 
                    // display player won prompt
                    puts("You won!");
                } else {
                    puts("You lost!");
                    // display player lost prompt
                }
                break;
            } else { 
                player_state = check_player_status(future_position, future_obstacle, map);
                if (player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->rect.x += 44;
            break;

    }

}


Map* init_map() {

    time_t t;
    srand((unsigned) time(&t));

    Map* map = malloc(1 * sizeof(Map));
    if (!map) {
        puts("Memory allocation for struct Map failed");
    }

    map->total_player_score = 0;
    map->number_of_blocks = NUMBER_OF_BLOCKS;
    map->number_of_foods = 2 + rand() % 9;

    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            map->board[row][col] = EMPTY;
        }
    }

    int exit_coordinate_x = 1 + rand() % 9;
    int exit_coordinate_y = 1 + rand() % 9;
    printf("exit coordinate %d %d\n", exit_coordinate_x, exit_coordinate_y);

    map->board[exit_coordinate_x][exit_coordinate_y] = EXIT;

    fill_board_with_blocks(map);
    fill_board_with_food(map);




    return map;
}

