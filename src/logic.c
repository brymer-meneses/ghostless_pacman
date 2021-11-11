
#include "stdbool.h"
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
int NUMBER_OF_FOODS = 8;
static int PLAYER_SCORE = 0;

static PlayerState PLAYER_STATE = PLAYER_STILL_PLAYING;



// void display_board(Sprite *pacman, Sprite *grid) {
void display_board(GameSprites* all_sprites) {

    Sprite* grid = &all_sprites->grid;
    Sprite* pacman = &all_sprites->pacman;
    SpriteCollection* foods = &all_sprites->foods;
    SpriteCollection* blocks = &all_sprites->blocks;

    draw_sprite(grid);
    draw_sprite(pacman);
    draw_sprite_collection(foods);
    draw_sprite_collection(blocks);

    int current_food_index = 0;
    int current_block_index = 0;

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
                        foods->rect[current_food_index].y =  ELEMENT_INITIAL_POSITION_Y + 5 +  (44 * col);
                        foods->rect[current_food_index].x =  ELEMENT_INITIAL_POSITION_X + 5 +  (44 * row);
                        current_food_index ++;
                        break;
                    case BLOCK:
                        blocks->rect[current_block_index].x =  ELEMENT_INITIAL_POSITION_X + 5 + (44 * row);
                        blocks->rect[current_block_index].y =  ELEMENT_INITIAL_POSITION_Y + 5 + (44 * col);
                        current_block_index ++;
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

void move_pacman(Move move, GameSprites* all_sprites) {
    Position current_position = query_pacman_position();
    int curr_x = current_position.x;
    int curr_y = current_position.y;

    Sprite* pacman = &all_sprites->pacman;

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

void handle_keypress(SDL_Event event, GameSprites* all_sprites) {
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
            move_pacman(MOVE_UP, all_sprites);
            break;
        case SDL_SCANCODE_A:
            move_pacman(MOVE_LEFT, all_sprites);
            break;
        case SDL_SCANCODE_S:
            move_pacman(MOVE_DOWN, all_sprites);
            break;
        case SDL_SCANCODE_D:
            move_pacman(MOVE_RIGHT, all_sprites);
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

GameSprites load_all_sprites(SDL_Renderer* renderer) {
    SDL_Rect pacman_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=34, .w=34};
    Sprite pacman = load_sprite(renderer, "assets/pacman.png", 40, 10, pacman_rect);

    SDL_Rect grid_rect = {GRID_POSITION_X, GRID_POSITION_Y, GRID_SIZE, GRID_SIZE};
    Sprite grid = load_sprite(renderer, "assets/grid.png", 0, 1, grid_rect);

    SDL_Rect exit_rect = {GRID_POSITION_X, GRID_POSITION_Y, 33, 23};
    Sprite exit = load_sprite(renderer, "assets/exit.png", 40, 10, exit_rect);

    SDL_Rect block_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=30, .w=30};
    SpriteCollection blocks = load_sprite_collection(NUMBER_OF_BLOCKS, renderer, "assets/box.png", 0, 1, block_rect);

    SDL_Rect food_rect = {.x =ELEMENT_INITIAL_POSITION_X, .y=ELEMENT_INITIAL_POSITION_Y, .h=27, .w=23};
    SpriteCollection foods = load_sprite_collection(NUMBER_OF_BLOCKS, renderer, "assets/food.png", 0, 1, food_rect);


    GameSprites sprites;
    sprites.pacman = pacman;
    sprites.grid = grid;
    sprites.exit = exit;
    sprites.blocks = blocks;
    sprites.foods = foods;

    fill_board_with_blocks();
    fill_board_with_food();

    return sprites;
}

void run_game(SDL_Renderer *renderer, GameSprites* all_sprites) {
    display_board(all_sprites);
}
