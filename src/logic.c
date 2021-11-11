
#include "stdbool.h"
#include "declarations.h"
#include "utils.h"

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

typedef enum PlayerState {
    PLAYER_WON,
    PLAYER_LOST,
    PLAYER_STILL_PLAYING,
} PlayerState;

static int MAP[10][10] = {
    {1, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0},
};

// TODO: Get number of food from user
// DEBUG: Set it to 8 for now
static int NUMBER_OF_FOODS = 8;
static int PLAYER_SCORE = 0;

static PlayerState PLAYER_STATE = PLAYER_STILL_PLAYING;



void display_board(Sprite *pacman, Sprite *grid) {
// void display_board(Sprite *pacman, Sprite* exit, Sprite *grid, Sprite blocks[NUMBER_OF_BLOCKS], Sprite foods[NUMBER_OF_FOODS]) {
    draw_sprite(grid);
    draw_sprite(pacman);
    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            BoardElement element = MAP[row][col];
                switch (element) {
                    case EMPTY:
                        continue;
                        break;
                    case PACMAN:
                        pacman->rect.y =  ELEMENT_INITIAL_POSITION_Y + (44 * row);
                        pacman->rect.x =  ELEMENT_INITIAL_POSITION_X + (44 * col);
                        break;
                    case FOOD:
                        // foods[row + col].rect.y =  ELEMENT_INITIAL_POSITION_Y + (44 * col);
                        // foods[row + col].rect.x =  ELEMENT_INITIAL_POSITION_X + (44 * row);
                        break;
                    case BLOCK:
                        // blocks[row + col].rect.x =  ELEMENT_INITIAL_POSITION_X + (44 * row);
                        // blocks[row + col].rect.y =  ELEMENT_INITIAL_POSITION_Y + (44 * col);
                        break;
                    case EXIT:
                        break;
                }
        }
    }
}

Position query_pacman_position() {
    Position pos;
    for (int row=0; row<10; row++) {
        for (int col=0; col<10; col++) {
            if (MAP[row][col] == PACMAN) {
                pos.x = row;
                pos.y = col;
                return pos;
            } 
            else {
                continue;
            }
        }
    }
};

void check_if_player_won() {
    if (PLAYER_SCORE == NUMBER_OF_FOODS) {
        PLAYER_STATE = PLAYER_WON;
    } else {
        PLAYER_STATE = PLAYER_LOST;
    }
};

void fill_board_with_food() {
    for (int i=0; i<NUMBER_OF_FOODS; i++) {
         int rand_x = rand() % 10;
         int rand_y = rand() % 10;
         MAP[rand_x][rand_y] = FOOD;
    }
}

void fill_board_with_blocks() {
    for (int i=0; i<NUMBER_OF_BLOCKS; i++) {
         int rand_x = rand() % 10;
         int rand_y = rand() % 10;
         MAP[rand_x][rand_y] = BLOCK;
    }
}

void move_pacman(Move move, Sprite *pacman) {
    Position current_position = query_pacman_position();
    int curr_x = current_position.x;
    int curr_y = current_position.y;

    int future_position;
    MAP[curr_x][curr_y] = EMPTY;
    switch (move) {
        case MOVE_UP:
            future_position = MAP[curr_x - 1][curr_y];
            MAP[curr_x - 1][curr_y] = PACMAN;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 270;
            break;
        case MOVE_DOWN:
            future_position = MAP[curr_x + 1][curr_y];
            MAP[curr_x + 1][curr_y] = PACMAN;

            pacman->flip = SDL_FLIP_VERTICAL;
            pacman->rotation = 90;
            break;
        case MOVE_LEFT:
            future_position = MAP[curr_x][curr_y -1];
            MAP[curr_x][curr_y - 1] = PACMAN;

            pacman->flip = SDL_FLIP_HORIZONTAL;
            pacman->rotation = 0;
            break;
        case MOVE_RIGHT:
            future_position = MAP[curr_x][curr_y +1];
            MAP[curr_x][curr_y + 1] = PACMAN;

            pacman->flip = SDL_FLIP_NONE;
            pacman->rotation = 0;
            break;

    }
    if (future_position == BLOCK) {
        PLAYER_STATE = PLAYER_LOST;
    } else if (future_position == FOOD) {
        PLAYER_SCORE++;
    }
}

void handle_keypress(SDL_Event event, Sprite* pacman) {
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
    switch (event.key.keysym.scancode) {
        // Moving pacman
        case SDL_SCANCODE_W:
            move_pacman(MOVE_UP, pacman);
            break;
        case SDL_SCANCODE_A:
            move_pacman(MOVE_LEFT, pacman);
            break;
        case SDL_SCANCODE_S:
            move_pacman(MOVE_DOWN, pacman);
            break;
        case SDL_SCANCODE_D:
            move_pacman(MOVE_RIGHT, pacman);
            break;

        // Misc
        case SDL_SCANCODE_M:
            break;
        default:
            // TODO: Warn the user if the input is invalid
            // perhaps add some instructions?
            break;
    }
}


