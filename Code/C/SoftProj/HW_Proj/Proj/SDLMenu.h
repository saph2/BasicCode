#define SDLMENU_H

#include "chessprog.h"
#include "Game.h"
#include "Moves.h"
#include "files.h"
#include "SDLControl.h"
#include "SDLGame.h"

int create_main_window();
int create_game_mode_window();
int valid_square_color(int j, int i, char *color);
char * get_picture(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
int present_board(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window);
int get_col_Index(int spot);
int get_row_Index(int spot);
int get_col_Square(int spot);
int get_row_Square(int spot);
int on_the_board(int j, int i);
int game_on_sdl();
int game_panels(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window, int row, int *game_mode, int *current_turn, int *user_color, int *difficulty, int *game_over, int *best);