#define SDLGAME_H

#include "chessprog.h"
#include "Game.h"
#include "Moves.h"
#include "files.h"
#include "SDLControl.h"

void restartValues(int *gameMode, int *currentTurn, int *userColor, int *diff, int *gameOver, int *bestDiff);
int show_possible_moves(int j, int i, char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window);
int mark_selected(int j, int i, char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window, char *color);
int choose_new_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList *userMove, int col, int row, SDL_Surface *game_window, char *color);
int make_move(char board[BOARD_SIZE][BOARD_SIZE], MovesList **userMove, int col, int row, SDL_Surface *game_window, char *color);
int choose_difficulty(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_setting_win, int j, int i);
int choose_color(SDL_Surface *game_setting_win, int j, int i);
int game_on_AI(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window);
int game_on_TwoPlayer(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window);
int start_game(char board[BOARD_SIZE][BOARD_SIZE]);
int start_setting(char board[BOARD_SIZE][BOARD_SIZE]);
int choose_game_mode(char board[BOARD_SIZE][BOARD_SIZE]);
int save_load_game(char save_or_load, char board[BOARD_SIZE][BOARD_SIZE]);