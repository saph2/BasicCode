#define MOVES_H_

#include "chessprog.h"

struct Move* init_move(int j, int i);
struct MovesList* init_movesList(int j, int i);
void free_movesList(struct MovesList* moves);
char *piece_charToString(char c);
char piece_stringToChar(char *piece, char *userColor);
void print_Moves(struct MovesList* moves);
void print_userMoves(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color);
int isLegal(int j, int i);
void getMoves_pawn_byDelta(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i, int j_Delta, int i_Delta);
void getMoves_Pawn(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
int verifyColor(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char *color);
void getMoves_byDelta(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i, int j_delta, int i_delta);
void getMoves_Rook(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
void getMoves_Bishop(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
void getMoves_Knight(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
void getMoves_Queen(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
void getMoves_King(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
void getMoves_Single(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i);
int isValidMove(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList *userMove, char *color, int console);
int create_userMove(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList *move, char* color);
void clear_kingThreatenningMoves(char board[BOARD_SIZE][BOARD_SIZE], MovesList *userMoves);
void changeBoard_SingleMove(char board[BOARD_SIZE][BOARD_SIZE], char newBoard[BOARD_SIZE][BOARD_SIZE], int from_col, int from_row, struct Move *destination);