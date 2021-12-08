/*
 * This file, `logic.c`, contains functions that handle the different
 * processes in the game itself
 */

#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#include "declarations.h"
#include "utils.h"
#include "logic.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

void render_board(Board *board, Assets* assets) {

    /* 
     * A function that renders the map of the game including
     * the main board which applies the concept of an array,
     * defining a 10-by-10 grid for the game. This function
     * includes rendering the sprites or visual assets inside
     * the said dimension, that is, Pacman itself, cherries,
     * blocks, and the exit door.
     * 
     * params:
     *      Assets* assets
     *          - A pointer to the struct Assets that points
     *            to the required assets
     *      Board *board   
     *          - A pointer to the struct Board that contains
     *            the necessary items to render the map
     */

    Sprite* main   =   &assets->game.main;
    Sprite* pacman =   &assets->game.pacman;
    Sprite* exit   =   &assets->game.exit;

    Sprite* foods  =   assets->game.foods;
    Sprite* blocks =   assets->game.blocks;

    render_sprite(main);
    render_sprite(exit);
    render_sprite(pacman);

    int current_food_index  = 0;
    int current_block_index = 0;

    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            enum BoardElement element = board->array[row][col];
                switch (element) {
                    case EMPTY:
                        break;
                    case PACMAN:
                        pacman->rect.x = ELEMENT_INITIAL_POSITION_X + (45 * col);
                        pacman->rect.y = ELEMENT_INITIAL_POSITION_Y + (45 * row);
                        break;
                    case FOOD:
                        foods[current_food_index].rect.x =  ELEMENT_INITIAL_POSITION_X  +  (45 * col);
                        foods[current_food_index].rect.y =  ELEMENT_INITIAL_POSITION_Y  +  (45 * row);
                        render_sprite(&foods[current_food_index]);
                        current_food_index++;
                        break;
                    case BLOCK:
                        blocks[current_block_index].rect.x =  ELEMENT_INITIAL_POSITION_X  + (45 * col);
                        blocks[current_block_index].rect.y =  ELEMENT_INITIAL_POSITION_Y  + (45 * row);
                        render_sprite(&blocks[current_block_index]);
                        current_block_index++;
                        break;
                    case EXIT:
                        exit->rect.x = ELEMENT_INITIAL_POSITION_X  + (45 * col);
                        exit->rect.y = ELEMENT_INITIAL_POSITION_Y  + (45 * row);
                        break;
                }
        }
    }
}

BoardPosition calculate_pacman_position(Sprite* pacman) {

    /* 
     * A helper function that calculates and returns the position of Pacman 
     * 
     * params:
     *      Sprite *pacman
     *          - a sprite containing pacman
     *  
     */

    BoardPosition position;
    position.col = (pacman->rect.x - ELEMENT_INITIAL_POSITION_X) / 45;
    position.row = (pacman->rect.y - ELEMENT_INITIAL_POSITION_Y) / 45;
    return position;
};


enum GameState check_player_status(BoardPosition next_position, enum BoardElement future_obstacle, Board *board, Assets* assets) {

    /* 
     * A function that dictates the state of the game based on the
     * current player status, specifically involving the blocks and
     * food
     */

    int row = next_position.row;
    int col = next_position.col;

    bool player_within_horizontal_borders = col >= 0 && col <=9;
    bool player_within_vertical_borders = row >= 0 && row <= 9;
    bool player_within_borders = player_within_horizontal_borders && player_within_vertical_borders;

    switch (future_obstacle) {
        case FOOD:
            board->total_player_score++;
            board->array[row][col] = EMPTY;
            Mix_PlayChannel(2, assets->sounds.pacman_munch, 0);
            break;
        case BLOCK:
            Mix_PlayChannel(2, assets->sounds.game_over, 0);
            return GAME_LOST_HIT_BLOCK;
            break;
        default: 
            break;
    }

    if (!player_within_borders) {
        Mix_PlayChannel(2, assets->sounds.game_over, 0);
        return GAME_LOST_HIT_BORDER;
    } 

    return GAME_IN_PROGRESS;
};

enum GameState check_if_player_won(BoardPosition next_position, Board* board, Assets* assets) {
    /*
     *  
     */

    // Check if the pacman has eaten all the foods
    if (board->total_player_score == board->number_of_foods) {
        Mix_PlayChannel(-1, assets->sounds.game_win, 0);
        return GAME_WON;
    } else {
        Mix_PlayChannel(-1, assets->sounds.game_over, 0);
        return GAME_LOST_INSUFFICIENT_FOOD;
    }

}

void fill_board_with_foods(Board *board) {
    /*
     *
     */
    int total_foods_generated = 0;
    int rand_row, rand_col;
    while (total_foods_generated < board->number_of_foods){
        // Generates random number between 0-9
         rand_row =  gen_random_num(0,9); 
        // Generates random number between 0-9
         rand_col =  gen_random_num(0,9);

         if (board->array[rand_row][rand_col] == EMPTY) {

            board->array[rand_row][rand_col] = FOOD;
            total_foods_generated++;
        }
    }
}

void fill_board_with_blocks(Board *board) {
    int total_blocks_generated = 0;
    int rand_row;
    int rand_col;
    while (total_blocks_generated < board->number_of_blocks) {
         // Prevents generating blocks that are adjacent to the 
         // border of the map.
         rand_row =  gen_random_num(1,8); // Generate random number between 1-8
         rand_col =  gen_random_num(1,8); // Generate random number between 1-8
         if (board->array[rand_row][rand_col] == EMPTY && rand_row != 0 && rand_col != 0) {

             board->array[rand_row][rand_col] = BLOCK;
             total_blocks_generated++;
        }
    }
}

void move_pacman(enum Move move, Assets *assets, Board* board, States *states) {

    Sprite* pacman = &assets->game.pacman;

    // Current position of pacman
    BoardPosition current_position = calculate_pacman_position(pacman);
    // Next position of pacman when the player moves
    BoardPosition next_position;

    enum BoardElement future_obstacle;
    switch (move) {
        case MOVE_UP:
            next_position.row =  current_position.row -  1;
            next_position.col =  current_position.col +  0; 

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;

            future_obstacle = board->array[next_position.row][next_position.col];
            if (future_obstacle == EXIT) { 
                states->game_state = check_if_player_won(next_position, board, assets);
                break;
            } else { 
                states->game_state = check_player_status(next_position, future_obstacle, board, assets);
                if (states->game_state != GAME_IN_PROGRESS) 
                    break;
            }


            board->array[current_position.row][current_position.col] = EMPTY;
            board->array[next_position.row][next_position.col] = PACMAN;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_DOWN:
            next_position.row =  current_position.row +  1; 
            next_position.col =  current_position.col +  0;

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;

            future_obstacle = board->array[next_position.row][next_position.col];
            if (future_obstacle == EXIT) { 
                states->game_state = check_if_player_won(next_position, board, assets);
                break;
            } else { 
                states->game_state = check_player_status(next_position, future_obstacle, board, assets);
                if (states->game_state != GAME_IN_PROGRESS) 
                    break;
            }

            board->array[current_position.row][current_position.col] = EMPTY;
            board->array[next_position.row][next_position.col] = PACMAN;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_LEFT:
            next_position.row =  current_position.row + 0;
            next_position.col =  current_position.col - 1; 

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;

            future_obstacle = board->array[next_position.row][next_position.col];
            if (future_obstacle == EXIT) { 
                states->game_state = check_if_player_won(next_position, board, assets);
                break;
            } else { 
                states->game_state = check_player_status(next_position, future_obstacle, board, assets);
                if (states->game_state != GAME_IN_PROGRESS) 
                    break;
            }

            board->array[current_position.row][current_position.col] = EMPTY;
            board->array[next_position.row][next_position.col] = PACMAN;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;
        case MOVE_RIGHT:
            next_position.col =  current_position.col + 1; 
            next_position.row =  current_position.row + 0;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;

            future_obstacle = board->array[next_position.row][next_position.col];
            if (future_obstacle == EXIT) { 
                states->game_state = check_if_player_won(next_position, board, assets);
            } else { 
                states->game_state = check_player_status(next_position, future_obstacle, board, assets);
                if (states->game_state != GAME_IN_PROGRESS) 
                    break;
            }

            board->array[current_position.row][current_position.col] = EMPTY;
            board->array[next_position.row][next_position.col] = PACMAN;
            Mix_PlayChannel(-1, assets->sounds.pacman_step, 0);
            break;

    };
}

bool check_for_impossible_win_scenario(Board* board) {
    /*
     * A function that checks for instances when
     * the randomly generated map is impossibe to be won.
     *  
     * params:
     *      Board *board
     *          - a pointer to the struct `Board` which
     *            contains the variables regarding the game
     * returns:
     *      true
     *          - if there is a possibility that the randomly
     *            generated map contains an impossible win
     *            scenario.
     *      false
     *          - if the conditions are not met.
     */

    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            enum BoardElement element = board->array[row][col];
            if (element == FOOD || element == EXIT) { 
                /* 
                 *  We define `impassable_adjacent_neighbors` as the number of
                 *  adjacent spaces around a food piece or an exit that is impossible
                 *  to be passed through. This includes adjacent block or touching the
                 *  borders.
                 *
                 *  Legend: 
                 *      - x : block
                 *      - * : food
                 *      - - : border
                 *
                 *  Example:
                 *      x
                 *       *
                 *      ---
                 *  This amounts to 4 impassable_adjacent_neighbors. 
                 *  
                 */

                int impassable_adjacent_neighbors = 0;
                for (int i=-1; i<=1; i++) {
                    for (int j=-1; j<=1; j++) {
                        // Counts the number of adjacent spaces exposed to the border
                        if (row + i == 0 || row + i == 9 || col + j == 0 || col + j == 9) {
                            impassable_adjacent_neighbors++;
                        // Counts the number of adjacent blocks or exit
                        } else if (board->array[row + i][col + j] == BLOCK || board->array[row + i][col + j] == EXIT) 
                            impassable_adjacent_neighbors++;
                        }
                } 
                if (impassable_adjacent_neighbors >= 4) 
                    return false;
                }
            }

        }
    return true;
}

void init_board(Board* board, Assets* assets, int number_of_foods) {

    // Ensures the board is empty
    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            board->array[row][col] = EMPTY;
        }
    }

    // Reset the position of pacman
    board->array[0][0] = PACMAN;
    assets->game.pacman.rect.x = ELEMENT_INITIAL_POSITION_X;
    assets->game.pacman.rect.y = ELEMENT_INITIAL_POSITION_Y;
    // Reset the rotation and flip of pacman
    assets->game.pacman.flip = SDL_FLIP_NONE;
    assets->game.pacman.rotation = 0;

    board->total_player_score = 0;
    board->number_of_blocks = NUMBER_OF_BLOCKS;
    board->number_of_foods = number_of_foods;

    // NOTE: Restricting the random number generated for row and col
    //       of the exit coordinate ensures that pacman will not be
    //       adjacent to the exit.

    // Generate random number between 1-9 and assign it
    int exit_coordinate_row = gen_random_num(1,9); 
    // Generate random number between 1-9 and assign it
    int exit_coordinate_col = gen_random_num(1,9); 

    board->array[exit_coordinate_row][exit_coordinate_col] = EXIT;

    fill_board_with_blocks(board);
    fill_board_with_foods(board);

    bool is_win_scenario_possible = check_for_impossible_win_scenario(board);

    // Initialize pacman again when an impossible win scenario may occur
    if (!is_win_scenario_possible) {
        init_board(board, assets, number_of_foods);
    }
}
