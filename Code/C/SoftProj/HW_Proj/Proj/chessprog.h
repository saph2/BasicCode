#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <ctype.h>
//#include <SDL.h>
#include <SDL_image.h>


#define BLACK_ROOK 'R'
#define WHITE_ROOK 'r'
#define BLACK_PAWN 'M'
#define WHITE_PAWN 'm'
#define BLACK_BISHOP 'B'
#define WHITE_BISHOP 'b'
#define BLACK_KNIGHT 'N'
#define WHITE_KNIGHT 'n'
#define BLACK_QUEEN 'Q'
#define WHITE_QUEEN 'q'
#define BLACK_KING 'K'
#define WHITE_KING 'k'
#define EMPTY ' '


#define BOARD_SIZE 8

typedef char** board_t;

#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
#define WRONG_GAME_MODE "Wrong game mode\n"
#define WRONG_COLOR "Wrong color. the color should be 'black' or 'white'\n"

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name))
#define SCANF_ERROR (perror("Error: standard function scanf has failed\n")
#define FPRINTF_ERROR (perror("Error: standard function scanf has failed\n")
#define print_message(message) (printf("%s", message));

struct Move{
	int row;
	int col;
	char changeTo;
	struct Move *next;
} typedef Move;

struct MovesList{
	struct Move *head;
	struct Move *tail;
} typedef MovesList;

void print_board_(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board_(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void print_board_(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board_(char board[BOARD_SIZE][BOARD_SIZE]);
void clear(char board[BOARD_SIZE][BOARD_SIZE]);
void set(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char *color, char *type);
void rm(char board[BOARD_SIZE][BOARD_SIZE], int j, int i);
int save_game(char board[BOARD_SIZE][BOARD_SIZE], int gameState, int difficulty, int best, int color, int nextTurn, char *file_chosen);
int load_game(char board[BOARD_SIZE][BOARD_SIZE], int *gameMode, int *difficulty, int *best, int *color, int *currentTurn, char * file_chosen);
int computer_vs_best(char board[BOARD_SIZE][BOARD_SIZE]);
char* gameState_twoPlayers(char board[BOARD_SIZE][BOARD_SIZE]);
char* gameState_player_vs_AI(char board[BOARD_SIZE][BOARD_SIZE]);
char* game_AI_setting(char board[BOARD_SIZE][BOARD_SIZE]);
char* game_setting(char board[BOARD_SIZE][BOARD_SIZE]);
void game_on();
#endif

int main(int argc, char* argv[]);
