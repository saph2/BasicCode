#define GAME_H_

#include "chessprog.h"
#include "Moves.h"

int scoringFunc_best(char board[BOARD_SIZE][BOARD_SIZE], char* color);
int scoringFunc(char board[BOARD_SIZE][BOARD_SIZE], char* color);
int checkForEnemy(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, int j_delta, int i_delta, char direction);
int checkForChess(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int checkForCheckmate(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int alphaBetaMax(int alpha, int beta, int depthleft, int currDepth, char board[BOARD_SIZE][BOARD_SIZE], char *color, struct MovesList *myBestMove, int best);
int alphaBetaMin(int alpha, int beta, int depthleft, int currDepth, char board[BOARD_SIZE][BOARD_SIZE], char *color, struct MovesList *myBestMove, int best);
int rating(char board[BOARD_SIZE][BOARD_SIZE], char *color, char *enemy_color);
int ratePieces(char board[BOARD_SIZE][BOARD_SIZE], char *color);
int ratePositional(char board[BOARD_SIZE][BOARD_SIZE], char *color, int pieces);
int rateMoveability(char board[BOARD_SIZE][BOARD_SIZE], char *color);