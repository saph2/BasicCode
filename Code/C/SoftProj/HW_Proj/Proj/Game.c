#include "Game.h"

int white_pawn_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 5, 10, 10, -20, -20, 10, 10, 5 }, { 5, -5, -10, 0, 0, -10, -5, 5 }, { 0, 0, 0, 20, 20, 0, 0, 0 }, { 5, 5, 10, 25, 25, 10, 5, 5 }, { 10, 10, 20, 30, 30, 20, 10, 10 }, { 50, 50, 50, 50, 50, 50, 50, 50 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int black_pawn_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 50, 50, 50, 50, 50, 50, 50, 50 }, { 10, 10, 20, 30, 30, 20, 10, 10 }, { 5, 5, 10, 25, 25, 10, 5, 5 }, { 0, 0, 0, 20, 20, 0, 0, 0 }, { 5, -5, -10, 0, 0, -10, -5, 5 }, { 5, 10, 10, -20, -20, 10, 10, 5 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int white_knight_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -30, -30, -30, -40, -50 }, { -40, -20, 0, 5, 5, 0, -20, -40 }, { -30, 5, 10, 15, 15, 10, 5, -30 }, { -30, 0, 15, 20, 20, 15, 0, -30 }, { -30, 5, 15, 20, 20, 15, 5, -30 }, { -30, 0, 10, 15, 15, 10, 0, -30 }, { -40, -20, 0, 0, 0, 0, -20, -40 }, { -50, -40, -30, -30, -30, -30, -40, -50 } };
int black_knight_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -30, -30, -30, -40, -50 }, { -40, -20, 0, 0, 0, 0, -20, -40 }, { -30, 0, 10, 15, 15, 10, 0, -30 }, { -30, 5, 15, 20, 20, 15, 5, -30 }, { -30, 0, 15, 20, 20, 15, 0, -30 }, { -30, 5, 10, 15, 15, 10, 5, -30 }, { -40, -20, 0, 5, 5, 0, -20, -40 }, { -50, -40, -30, -30, -30, -30, -40, -50 } };
int white_queen_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -5, -5, -10, -10, -20 }, { -10, 0, 5, 0, 0, 0, 0, -10 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { 0, 0, 5, 5, 5, 5, 0, -5 }, { -5, 0, 5, 5, 5, 5, 0, -5 }, { -10, 0, 5, 5, 5, 5, 0, -10 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -20, -10, -10, -5, -5, -10, -10, -20 } };
int black_queen_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -5, -5, -10, -10, -20 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { -5, 0, 5, 5, 5, 5, 0, -5 }, { 0, 0, 5, 5, 5, 5, 0, -5 }, { -10, 5, 5, 5, 5, 5, 0, -10 }, { -10, 0, 5, 0, 0, 0, 0, -10 }, { -20, -10, -10, -5, -5, -10, -10, -20 } };
int white_bishop_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -10, -10, -10, -10, -20 }, { -10, 5, 0, 0, 0, 0, 5, -10 }, { -10, 10, 10, 10, 10, 10, 10, -10 }, { -10, 0, 10, 10, 10, 10, 0, -10 }, { -10, 5, 5, 10, 10, 5, 5, -10 }, { -10, 0, 5, 10, 10, 5, 0, -10 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -20, -10, -10, -10, -10, -10, -10, -20 } };
int black_bishop_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { -20, -10, -10, -10, -10, -10, -10, -20 }, { -10, 0, 0, 0, 0, 0, 0, -10 }, { -10, 0, 5, 10, 10, 5, 0, -10 }, { -10, 5, 5, 10, 10, 5, 5, -10 }, { -10, 0, 10, 10, 10, 10, 0, -10 }, { -10, 10, 10, 10, 10, 10, 10, -10 }, { -10, 5, 0, 0, 0, 0, 5, -10 }, { -20, -10, -10, -10, -10, -10, -10, -20 } };
int white_rook_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 5, 5, 0, 0, 0 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { 5, 10, 10, 10, 10, 10, 10, 5 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
int black_rook_best_coordinates[BOARD_SIZE][BOARD_SIZE] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 5, 10, 10, 10, 10, 10, 10, 5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { -5, 0, 0, 0, 0, 0, 0, -5 }, { 0, 0, 0, 5, 5, 0, 0, 0 } };
int white_king_best_coordinates_start[BOARD_SIZE][BOARD_SIZE] = { { 20, 30, 10, 0, 0, 10, 30, 20 }, { 20, 20, 0, 0, 0, 0, 20, 20 }, { -10, -20, -20, -20, -20, -20, -20, -10 }, { -20, -30, -30, -40, -40, -30, -30, -20 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 } };
int black_king_best_coordinates_start[BOARD_SIZE][BOARD_SIZE] = { { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -30, -40, -40, -50, -50, -40, -40, -30 }, { -20, -30, -30, -40, -40, -30, -30, -20, }, { -10, -20, -20, -20, -20, -20, -20, -10 }, { 20, 20, 0, 0, 0, 0, 20, 20 }, { 20, 30, 10, 0, 0, 10, 30, 20 } };
int black_king_best_coordinates_end[BOARD_SIZE][BOARD_SIZE] = { { -50, -40, -30, -20, -20, -30, -40, -50 }, { -30, -20, -10, 0, 0, -10, -20, -30 },  { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 20, 30, 30, 20, -10, -30 },  { -30, -30, 0, 0, 0, 0, -30, -30 }, { -50, -30, -30, -30, -30, -30, -30, -50 } };
int white_king_best_coordinates_end[BOARD_SIZE][BOARD_SIZE] = { { -50, -30, -30, -30, -30, -30, -30, -50 }, { -30, -30, 0, 0, 0, 0, -30, -30 }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30 }, { -30, -10, 30, 40, 40, 30, -10, -30, }, { -30, -10, 20, 30, 30, 20, -10, -30 }, { -30, -20, -10, 0, 0, -10, -20, -30 }, { -50, -40, -30, -20, -20, -30, -40, -50 } };

int ratePieces(char board[BOARD_SIZE][BOARD_SIZE], char *color) {
	int i, j;
	int bishopCounter=0;
	int counter=0;
	if (strcmp(color, "black")==0){
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				 if (board[j][i] == BLACK_QUEEN)
					counter += 900;
				else if (board[j][i] == BLACK_KNIGHT)
					counter += 300;
				else if (board[j][i] == BLACK_BISHOP)
					counter += 300;
				else if (board[j][i] == BLACK_ROOK)
					counter += 500;
				else if (board[j][i] == BLACK_PAWN)
					counter += 100;
			}
		}
	}
	else{
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				 if (board[j][i] == WHITE_QUEEN)
					counter += 900;
				else if (board[j][i] == WHITE_KNIGHT)
					counter += 300;
				else if (board[j][i] == WHITE_BISHOP)
					counter += 300;
				else if (board[j][i] == WHITE_ROOK)
					counter += 500;
				else if (board[j][i] == WHITE_PAWN)
					counter += 100;
			}
		}
	}
	if (bishopCounter >= 2){
		counter += 300 * bishopCounter;
	}
	else{
		if (bishopCounter == 1){
			counter += 250;
		}
	}
	return counter;
}

int ratePositional(char board[BOARD_SIZE][BOARD_SIZE], char *color, int pieces) {
	int i, j;
	int counter=0;
	if (strcmp(color, "black")==0){
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == BLACK_QUEEN)
					counter += black_queen_best_coordinates[j][i];
				else if (board[j][i] == BLACK_KNIGHT)
					counter += black_knight_best_coordinates[j][i];
				else if (board[j][i] == BLACK_BISHOP)
					counter += black_bishop_best_coordinates[j][i];
				else if (board[j][i] == BLACK_ROOK)
					counter += black_rook_best_coordinates[j][i];
				else if (board[j][i] == BLACK_PAWN)
					counter += black_pawn_best_coordinates[j][i];
				else if (board[j][i] == BLACK_KING){
					if (pieces >= 1750){
						counter += black_king_best_coordinates_start[j][i];
					}
					else {
						counter += black_king_best_coordinates_end[j][i];
					}
				}
			}
		}
	}
	else{
		for (j = 0; j < BOARD_SIZE; ++j){
			for (i = 0; i < BOARD_SIZE; ++i){
				/*sum all the color's and enemy's pawns/kings*/
				if (board[j][i] == WHITE_QUEEN)
					counter += white_queen_best_coordinates[j][i];
				else if (board[j][i] == WHITE_KNIGHT)
					counter += white_knight_best_coordinates[j][i];
				else if (board[j][i] == WHITE_BISHOP)
					counter += white_bishop_best_coordinates[j][i];
				else if (board[j][i] == WHITE_ROOK)
					counter += white_rook_best_coordinates[j][i];
				else if (board[j][i] == WHITE_PAWN)
					counter += white_pawn_best_coordinates[j][i];
				else if (board[j][i] == WHITE_KING){
					if (pieces >= 1750){
						counter += white_king_best_coordinates_start[j][i];
					}
					else {
						counter += white_king_best_coordinates_end[j][i];
					}
				}
			}
		}
	}

	return counter;
}

int num_of_pieces_moves(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	int num_of_moves = 0;
	MovesList* moves;
	Move *move;
	int j, i;
	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){
			if (verifyColor(board, j, i, color) == 1){/*for each of the color's pawn/king check if it can move*/
				moves = init_movesList(j, i);
				getMoves_Single(board, moves, j, i);
				clear_kingThreatenningMoves(board, moves);
				move = moves->head;
				while (move != moves->tail){
					move = move->next;
					num_of_moves++;
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return num_of_moves;
}

int rateMoveability(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	int counter = 0;
	counter += num_of_pieces_moves(board, color);
	return counter;
}


int rating(char board[BOARD_SIZE][BOARD_SIZE], char *color, char *enemy_color){
	/*best - scoring func*/
	int counter = 0;
	int count_moveability_color;
	int count_moveability_enemycolor;
	int piecesRating = ratePieces(board, color);
	count_moveability_color = rateMoveability(board, color);
	count_moveability_enemycolor = rateMoveability(board, enemy_color);
	if (count_moveability_color == 0){
		return -200000;
	}
	if (count_moveability_enemycolor == 0){
		return 200000;
	}
	counter += piecesRating;
	counter += count_moveability_color;
	counter += ratePositional(board, color, piecesRating);
	piecesRating = ratePieces(board, enemy_color);
	counter -= piecesRating;
	counter -= count_moveability_enemycolor;
	counter -= ratePositional(board, enemy_color, piecesRating);
	return counter;
}

int scoringFunc(char board[BOARD_SIZE][BOARD_SIZE], char* color){
	/*return the min max function's result on the recieved board - [color's sum minus enemy's sum]*/
	int i, j;
	char *enemy_color;
	int sumB = 0;
	int sumW = 0;
	if (strcmp(color, "black") == 0){
		enemy_color = "white";
	}
	else{
		enemy_color = "black";
	}
	if (checkForCheckmate(board, color) == 1) {
		return 100000;
	}
	if (checkForCheckmate(board, enemy_color) == 1) {
		return -100000;
	}
	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){
			/*sum all the color's and enemy's pawns/kings*/
			if (board[j][i] == BLACK_KING)
				sumB += 400;
			else if (board[j][i] == WHITE_KING)
				sumW += 400;
			else if (board[j][i] == BLACK_QUEEN)
				sumB += 9;
			else if (board[j][i] == WHITE_QUEEN)
				sumW += 9;
			else if (board[j][i] == BLACK_KNIGHT || board[j][i] == BLACK_BISHOP)
				sumB += 3;
			else if (board[j][i] == WHITE_KNIGHT || board[j][i] == WHITE_BISHOP)
				sumW += 3;
			else if (board[j][i] == BLACK_ROOK)
				sumB += 5;
			else if (board[j][i] == WHITE_ROOK)
				sumW += 5;
			else if (board[j][i] == BLACK_PAWN)
				sumB += 1;
			else if (board[j][i] == WHITE_PAWN)
				sumW += 1;
		}
	}
	if (strcmp(color, "black") == 0){/*return min max func result for color minus enemy*/
		return (sumB - sumW);
	}
	else{
		return (sumW - sumB);
	}
}

int checkForEnemy(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, int j_delta, int i_delta, char direction){
	/*returns if an enemy in the index and diraction recieved can eat our piece in the index recieved*/
	int dis = 0;
	int orig_i = i;
	int orig_j = j;
	while (isLegal(j + j_delta, i + i_delta) == 1){ /*for down diraction*/
		if (board[j + j_delta][i + i_delta] == EMPTY && direction != 'L'){
			j += j_delta;
			i += i_delta;
			dis = 1;
		}
		else break;
	}
	if (isLegal(j + j_delta, i + i_delta) == 1 && board[j + j_delta][i + i_delta] != EMPTY){/*there is a pawn in the kings way*/
		if (verifyColor(board, orig_j, orig_i, "black") == 1){/*check for white enemys*/
			if (direction == 's'){/*straight*/
				if (board[j + j_delta][i + i_delta] == WHITE_QUEEN || board[j + j_delta][i + i_delta] == WHITE_ROOK)
					return 1;
				else if (dis == 0 && board[j + j_delta][i + i_delta] == WHITE_KING)
					return 1;
			}
			else if (direction == 'd'){/*digonal*/
				if (board[j + j_delta][i + i_delta] == WHITE_QUEEN || board[j + j_delta][i + i_delta] == WHITE_BISHOP)
					return 1;
				else if (dis == 0 && ((board[j - 1][i - 1] == WHITE_PAWN) || (board[j + 1][i - 1] == WHITE_PAWN) || board[j + j_delta][i + i_delta] == WHITE_KING))
					return 1;
			}
			else if (direction == 'L' && dis == 0){/*knight*/
				if (board[j + j_delta][i + i_delta] == WHITE_KNIGHT)
					return 1;
				else return 0;
			}
		}
		else{/*check for black enemys*/
			if (direction == 's'){/*straight*/
				if (board[j + j_delta][i + i_delta] == BLACK_QUEEN || board[j + j_delta][i + i_delta] == BLACK_ROOK)
					return 1;
				else if (dis == 0 && board[j + j_delta][i + i_delta] == BLACK_KING)
					return 1;
			}
			else if (direction == 'd'){/*digonal*/
				if (board[j + j_delta][i + i_delta] == BLACK_QUEEN || board[j + j_delta][i + i_delta] == BLACK_BISHOP)
					return 1;
				else if (dis == 0 && ((board[j - 1][i + 1] == BLACK_PAWN) || (board[j + 1][i + 1] == BLACK_PAWN) || board[j + j_delta][i + i_delta] == BLACK_KING))
					return 1;
			}
			else if (direction == 'L' && dis == 0){/*knight*/
				if (board[j + j_delta][i + i_delta] == BLACK_KNIGHT)
					return 1;
				else return 0;
			}

		}
	}
	return 0;
}

int checkForChess(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	/*returns 1 if the color can make a chess on his enemy*/
	int i, j;
	int index_i = -1;
	int index_j = -1;
	int chess = 0;
	for (j = 0; j < BOARD_SIZE; ++j){/*loop to find enemy's king*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (strcmp(color, "white") == 0 && board[j][i] == BLACK_KING){
				index_j = j;
				index_i = i;
				break;
			}
			else if (strcmp(color, "black") == 0 && board[j][i] == WHITE_KING){
				index_j = j;
				index_i = i;
				break;
			}
			if (index_j != -1)
				break;
		}
	}
	if (index_j == -1)/*no enemy king was found*/
		return 1;
	else{/*all possible options for chess*/
		chess = checkForEnemy(board, index_j, index_i, 0, 1, 's');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 0, -1, 's');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -1, 0, 's');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 1, 0, 's');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -1, -1, 'd');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -1, 1, 'd');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 1, -1, 'd');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 1, 1, 'd');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -1, -2, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -1, 2, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 1, -2, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 1, 2, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -2, -1, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, -2, 1, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 2, -1, 'L');
		if (chess == 1) return 1;
		chess = checkForEnemy(board, index_j, index_i, 2, 1, 'L');
		if (chess == 1) return 1;
		return 0;
	}
}

int checkForCheckmate(char board[BOARD_SIZE][BOARD_SIZE], char *color){
	/*returns 1 if the color can win the game*/
	int i, j;
	struct Move *curr;
	struct MovesList *moves;
	char newBoard[BOARD_SIZE][BOARD_SIZE];
	char *enemyCol;
	if (strcmp(color, "white") == 0){
		enemyCol = "black";
	}
	else{
		enemyCol = "white";
	}
	for (j = 0; j < BOARD_SIZE; ++j){
		for (i = 0; i < BOARD_SIZE; ++i){/*for every enemy's pawn on the board*/
			if (verifyColor(board, j, i, enemyCol) == 1){/*get enemy pawn's moves*/
				moves = init_movesList(-1, -1);
				getMoves_Single(board, moves, j, i);
				if (moves->head->next != NULL){/*if the rival can move*/
					for (curr = moves->head->next; curr != (moves->tail)->next; curr = curr->next) {/*go over each move - changed from source*/
						changeBoard_SingleMove(board, newBoard, moves->head->col, moves->head->row, curr);
						if (checkForChess(newBoard, color) == 0) {/*if the king can be saved*/
							free_movesList(moves);
							free(moves);
							return 0;
						}
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return 1;/*color wins - rival can't be saved or move*/
}


int alphaBetaMax(int alpha, int beta, int maxDept, int currDepth, char board[BOARD_SIZE][BOARD_SIZE], char *color, struct MovesList *myBestMove, int best) {
	/*maximizing MiniMaxFunction*/
	int i, j, score;
	struct Move *move;
	struct MovesList *moves;
	char *enemy_color;
	char newBoard[BOARD_SIZE][BOARD_SIZE];
	if (strcmp(color, "black") == 0) {
		enemy_color = "white";
	}
	else {
		enemy_color = "black";
	}
	if (currDepth == maxDept) {/*last level*/
		if (best == 0){
			return scoringFunc(board, color);
		}
		else{
			return rating(board, color,enemy_color);
		}
	}
	for (j = 0; j < BOARD_SIZE; ++j){/*go over each piece*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (verifyColor(board, j, i, color) == 1){
				moves = init_movesList(j, i);
				getMoves_Single(board, moves, j, i);/*get moves for piece*/
				clear_kingThreatenningMoves(board, moves);/*clear illegal moves*/
				move = moves->head;
				while (move != moves->tail){/*go over each move for this current piece*/
					move = move->next;
					changeBoard_SingleMove(board, newBoard, j, i, move);/*change board as a result of the current move*/
					score = alphaBetaMin(alpha, beta, maxDept, currDepth + 1, newBoard, enemy_color, myBestMove, best);
					if (alpha < score){
						alpha = score;
						if (currDepth == 0){/*get best move*/
							(myBestMove)->head->col = j;
							(myBestMove)->head->row = i;
							(myBestMove)->tail->col = move->col;
							(myBestMove)->tail->row = move->row;
							(myBestMove)->tail->changeTo = move->changeTo;
						}
					}
					if (beta <= alpha){/*cut when pruning found best result*/
						free_movesList(moves);
						free(moves);
						return alpha;
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return alpha;
}

int alphaBetaMin(int alpha, int beta, int maxDept, int currDepth, char board[BOARD_SIZE][BOARD_SIZE], char *color, struct MovesList *myBestMove, int best) {
	/*minimizing MiniMaxFunction*/
	int i, j, score;
	struct Move *move;
	struct MovesList *moves;
	char *enemy_color;
	char newBoard[BOARD_SIZE][BOARD_SIZE];
	if (strcmp(color, "black") == 0) {
		enemy_color = "white";
	}
	else {
		enemy_color = "black";
	}
	if (currDepth == maxDept) {/*last level*/
		if (best == 0){
			return scoringFunc(board, enemy_color);
		}
		else{
			return rating(board, enemy_color,color);
		}
	}
	for (j = 0; j < BOARD_SIZE; ++j){/*go over each piece*/
		for (i = 0; i < BOARD_SIZE; ++i){
			if (verifyColor(board, j, i, color) == 1){
				moves = init_movesList(j, i);
				getMoves_Single(board, moves, j, i);/*get moves for piece*/
				clear_kingThreatenningMoves(board, moves);/*clear illegal moves*/
				move = moves->head;
				while (move != moves->tail){/*go over each move for this current piece*/
					move = move->next;
					changeBoard_SingleMove(board, newBoard, j, i, move);/*change board as a result of the current move*/
					score = alphaBetaMax(alpha, beta, maxDept, currDepth + 1, newBoard, enemy_color, myBestMove, best);
					if (beta > score){
						beta = score;
					}
					if (beta <= alpha){/*cut when pruning found best result*/
						free_movesList(moves);
						free(moves);
						return beta;
					}
				}
				free_movesList(moves);
				free(moves);
			}
		}
	}
	return beta;
}