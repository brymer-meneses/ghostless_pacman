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
     * params
     *      Assets* assets
     *            A pointer to the struct Assets that points
     *            to the required assets
     *      Board *board   
     *            A pointer to the struct Board that contains
     *            the necessary items to render the map
     */

    // NOTE:
    //  Changing `y` and `x` of the struct SDL_Rect that is a member of the
    //  struct Sprite, will cause the image encapsulatd in the Sprite to move
    //  depending on how much you increase or decrease these variables.

    Sprite* main   =   &assets->game.main;
    Sprite* pacman =   &assets->game.pacman;
    Sprite* exit   =   &assets->game.exit;

    // NOTE:
    //  The variable `foods` and `blocks` points into an array containing sprites.
    //  this is it is important to iterate through the members of this array and 
    //  render the accordingly.

    Sprite* foods  =   assets->game.foods;
    Sprite* blocks =   assets->game.blocks;

    int current_food_index  = 0;
    int current_block_index = 0;

    render_sprite(main);
    render_sprite(exit);
    render_sprite(pacman);


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
     * A helper function that calculates and returns the position of Pacman.
     * 
     * params
     *      Sprite *pacman
     *          a pointer to a sprite containing pacman
     */

    BoardPosition position;
    position.col = (pacman->rect.x - ELEMENT_INITIAL_POSITION_X) / 45;
    position.row = (pacman->rect.y - ELEMENT_INITIAL_POSITION_Y) / 45;
    return position;
};


enum GameState check_player_status(BoardPosition next_position, enum BoardElement future_obstacle, Board *board, Assets* assets) {

    /* 
     * A function that checks the state of the game based on the
     * current player status, specifically involving the blocks and
     * food
     *
     * params
     *      BoardPosition next_position
     *          A struct holding the next position of pacman
     *      enum BoardElement future_obstace
     *          An enum that represents the future obstacle that pacman will
     *          encounter on its `next_position`
     *      Board *board
     *          A pointer to the struct board which stores all the variables
     *          related to the game itself.
     * returns
     *      enum GameState
     *          the `state` of the game when pacman moves into the `next_position`. 
     *
     * example
     *      If pacman moves into a block on its `next_position`, this function
     *      will return a `GAME_LOST_HIT_BLOCK` enum.
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

enum GameState check_if_player_won(BoardPosition next_position, Board *board, Assets *assets) {

    /* 
     * A function that checks if the player won in the game.
     *
     * params
     *      BoardPosition next_position
     *          A struct holding the next position of pacman on the board.
     *      Board *board
     *          A pointer to the struct board which stores all the variables
     *          related to the game itself.
     *      Assets *assets
     *          A pointer to the struct Assets that points
     *          to the required assets
     * returns
     *      enum GameState
     *          the `state` of the game when pacman moves into the `next_position`. 
     *
     * example
     *      If pacman moves into the exit on its `next_position`, and pacman has 
     *      eaten all the fruit. This function will return a game_state enum
     *      PLAYER_WON.
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

void move_pacman(enum PacmanMove pacman_move, Assets *assets, Board* board, States *states) {

    /* 
     * A function that handels the movement of pacman
     * current player status, specifically involving the blocks and
     * food
     *
     * params
     *      BoardPosition next_position
     *          A struct holding the next position of pacman
     *      enum BoardElement future_obstace
     *          An enum that represents the future obstacle that pacman will
     *          encounter on its `next_position`
     *      Board *board
     *          A pointer to the struct board which stores all the variables
     *          related to the game itself.
     * returns
     *      enum GameState
     *          the `state` of the game when pacman moves into the `next_position`. 
     *
     * example
     *      If pacman moves into a block on its `next_position`, this function
     *      will return a `GAME_LOST_HIT_BLOCK` enum.
     */
    Sprite* pacman = &assets->game.pacman;

    // Current position of pacman
    BoardPosition current_position = calculate_pacman_position(pacman);
    // Next position of pacman when the player moves
    BoardPosition next_position;

    enum BoardElement future_obstacle;
    switch (pacman_move) {
        case MOVE_PACMAN_UP:
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
        case MOVE_PACMAN_DOWN:
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
        case MOVE_PACMAN_LEFT:
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
        case MOVE_PACMAN_RIGHT:
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

int count_impassable_neighbors(Board* board, int row, int col) {
    /* 
     * A function that calculates the number of impassable neighbors
     * there are in the given `row` and `col` variables.
     *  
     * params:
     *      Board *board
     *          - a pointer to the struct `Board` which
     *            contains the variables regarding the game
     * returns:
     *      
     *  NOTE:
     *
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
    return impassable_adjacent_neighbors;
}

void fill_board_with_foods(Board *board) {
    /* 
     * A function that randomly fills the board with food
     * 
     * params
     *       Board *board
     *          A pointer to the struct Board
     *
     */
    int total_foods_generated = 0;
    int rand_row, rand_col;
    int number_of_impassable_neighbors;
    while (total_foods_generated < board->number_of_foods){
         // Generates random number between 0-9
         rand_row =  gen_random_num(0,9); 
         rand_col =  gen_random_num(0,9);

         number_of_impassable_neighbors = count_impassable_neighbors(board, rand_row, rand_col);

         if (
            board->array[rand_row][rand_col] == EMPTY &&
            number_of_impassable_neighbors <= MAX_ADJACENT_IMPASSABLE_NEIGHBORS
         ) {
            board->array[rand_row][rand_col] = FOOD;
            total_foods_generated++;
        }
    }
}

void fill_board_with_blocks(Board *board) {
    /*
     * A function that fills the board with blocks
     * 
     * params
     *       Board *board
     *          A pointer to the struct Board
     *
     */
    int total_blocks_generated = 0;
    int rand_row;
    int rand_col;
    while (total_blocks_generated < board->number_of_blocks) {

         // NOTE:
         //   To decrease the chances of an impossible win scenario, the following
         //   restricts the random generation of row and column index wihin the 
         //   range [1,8].
         rand_row =  gen_random_num(1,8); // Generate random number between 1-8
         rand_col =  gen_random_num(1,8); // Generate random number between 1-8
         if (board->array[rand_row][rand_col] == EMPTY && rand_row != 0 && rand_col != 0) {

             board->array[rand_row][rand_col] = BLOCK;
             total_blocks_generated++;
        }
    }
}

void fill_board_with_exit(Board *board) {
    /*
     * A function that randomly fills the board with an exit
     * 
     * params
     *       Board *board
     *          A pointer to the struct Board which holds
     *          variables that are concerned with the game itself.
     */

    // NOTE: 
    //  Restricting the random number generated for row and col
    //  to the range [1, 9] ensures that that pacman will not be
    //  adjacent to the exit.

    // Generate random number between 1-9
    int rand_row = gen_random_num(1,9); 
    int rand_col = gen_random_num(1,9); 
    int number_impassable_neighbors = count_impassable_neighbors(board, rand_col, rand_row);

    while (number_impassable_neighbors > MAX_ADJACENT_IMPASSABLE_NEIGHBORS) {
        rand_row = gen_random_num(1,9); 
        rand_col = gen_random_num(1,9); 
        number_impassable_neighbors = count_impassable_neighbors(board, rand_col, rand_row);
    } 

    board->array[rand_row][rand_col] = EXIT;
}

void init_board(Board* board, Assets* assets, int number_of_foods) {

    // Reset the board array
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

    // Reset the board
    board->total_player_score = 0;
    board->number_of_blocks = NUMBER_OF_BLOCKS;
    board->number_of_foods = number_of_foods;

    fill_board_with_blocks(board);
    fill_board_with_exit(board);
    fill_board_with_foods(board);
}
