
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#include "declarations.h"
#include "utils.h"
#include "logic.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

void render_map(Map *map, Assets* assets) {

    Sprite* grid =   &assets->game.grid;
    Sprite* pacman = &assets->game.pacman;
    Sprite* exit =   &assets->game.exit;

    Sprite* foods =   assets->game.foods;
    Sprite* blocks =  assets->game.blocks;

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


PlayerState check_player_status(Position future_position, BoardElement future_obstacle, Map *map, Assets* assets) {
    int x = future_position.x;
    int y = future_position.y;

    bool player_within_horizontal_borders = y >= 0 && y <=9;
    bool player_within_vertical_borders = x >= 0 && x <= 9;
    bool player_within_borders = player_within_horizontal_borders && player_within_vertical_borders;

    switch (future_obstacle) {
        case FOOD:
            map->total_player_score++;
            map->board[x][y] = EMPTY;
            Mix_PlayChannel(-1, assets->sounds.pacman_munch, 0);
            break;
        case BLOCK:
            Mix_PlayChannel(-1, assets->sounds.game_over, 0);
            return PLAYER_LOST_HIT_BLOCK;
            break;
        default: 
            break;
    }

    if (!player_within_borders) {
        Mix_PlayChannel(-1, assets->sounds.game_over, 0);
        return PLAYER_LOST_HIT_BORDER;
    } 

    return PLAYER_STILL_PLAYING;
};

PlayerState check_if_player_won(Position future_position, Map* map, Assets* assets) {
    if (map->total_player_score == map->number_of_foods) {
        Mix_PlayChannel(-1, assets->sounds.game_win, 0);
        return PLAYER_WON;
    } else {
        Mix_PlayChannel(-1, assets->sounds.game_over, 0);
        return PLAYER_LOST_INSUFFICIENT_FOOD;
    }

}

void fill_board_with_food(Map *map) {
    int total_foods_generated = 0;
    int rand_x;
    int rand_y;
    while (total_foods_generated < map->number_of_foods){
         rand_x =  gen_random_num(0, 9);
         rand_y =  gen_random_num(0,9);
         if (map->board[rand_x][rand_y] == EMPTY && rand_x != 0 && rand_y != 0) {
            map->board[rand_x][rand_y] = FOOD;
            total_foods_generated++;
        }
    }
}

void fill_board_with_blocks(Map *map) {
    int total_blocks_generated = 0;
    int rand_x;
    int rand_y;
    while (total_blocks_generated < map->number_of_blocks) {
         rand_x =  gen_random_num(0, 9);
         rand_y =  gen_random_num(0,9);
         if (map->board[rand_x][rand_y] == EMPTY && rand_x != 0 && rand_y != 0) {
             map->board[rand_x][rand_y] = BLOCK;
             total_blocks_generated++;
        }
    }
}

void move_pacman(Move move, Assets *assets, Map* map, PlayerState *player_state) {

    Sprite* pacman = &assets->game.pacman;

    Position current_position = query_pacman_position(pacman);
    Position future_position;

    PlayerState future_player_state;

    BoardElement future_obstacle;
    switch (move) {
        case MOVE_UP:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y + -1;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                future_player_state = check_if_player_won(future_position, map, assets);
            } else { 
                future_player_state = check_player_status(future_position, future_obstacle, map, assets);
                if (future_player_state != PLAYER_STILL_PLAYING) break;
            }


            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;

            pacman->rect.y -= 44;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_DOWN:
            future_position.x =  current_position.x +  0; 
            future_position.y =  current_position.y +  1;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                future_player_state = check_if_player_won(future_position, map, assets);
            } else { 
                future_player_state = check_player_status(future_position, future_obstacle, map, assets);
                if (future_player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;

            pacman->rect.y += 44;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_LEFT:
            future_position.x =  current_position.x + -1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                future_player_state = check_if_player_won(future_position, map, assets);
                break;
            } else { 
                future_player_state = check_player_status(future_position, future_obstacle, map, assets);
                if (future_player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->rect.x -= 44;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_RIGHT:
            future_position.x =  current_position.x +  1; 
            future_position.y =  current_position.y +  0;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;

            future_obstacle = map->board[future_position.x][future_position.y];
            if (future_obstacle == EXIT) { 
                *player_state = check_if_player_won(future_position, map, assets);
            } else { 
                future_player_state = check_player_status(future_position, future_obstacle, map, assets);
                if (future_player_state != PLAYER_STILL_PLAYING) break;
            }

            pacman->rect.x += 44;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;

    }
    // Update `player_state` located on `main.c`
    *player_state = future_player_state;

}

void reset_map(Map* map, Assets* assets) {

    // make sure the board is empty
    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            map->board[row][col] = EMPTY;
        }
    }

    // reset the position of pacman
    assets->game.pacman.rect.x = ELEMENT_INITIAL_POSITION_X;
    assets->game.pacman.rect.y = ELEMENT_INITIAL_POSITION_Y;

    map->total_player_score = 0;
    map->number_of_blocks = NUMBER_OF_BLOCKS;
    map->number_of_foods = gen_random_num(2, 9);

    int exit_coordinate_x = gen_random_num(1, 9);
    int exit_coordinate_y = gen_random_num(1,9);

    map->board[exit_coordinate_x][exit_coordinate_y] = EXIT;

    fill_board_with_blocks(map);
    fill_board_with_food(map);
}


Map* init_map(Assets *assets) {

    time_t t;
    srand((unsigned) time(&t));

    Map* map = malloc(1 * sizeof(Map));
    if (!map) {
        puts("Memory allocation for struct Map failed");
    }

    reset_map(map, assets);

    return map;
}


