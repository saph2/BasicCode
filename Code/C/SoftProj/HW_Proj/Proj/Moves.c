#include "Moves.h"

struct Move* init_move(int j, int i) {
	/*initializes a new move*/
	struct Move *move = (struct Move*) malloc(sizeof(struct Move));
	if (move == NULL) {
		perror("Error: standard function malloc has failed");
		exit(0);
	}
	(*move).col = j;
	(*move).row = i;
	(*move).next = NULL;
	(*move).changeTo = 'd'; /*d=default, don't change*/
	return move;

}

struct MovesList* init_movesList(int j, int i) {
	/*initializes moves list*/
	struct MovesList *moves = (struct MovesList*) malloc(sizeof(struct MovesList));
	if (moves == NULL) {
		perror("Error: standard function malloc has failed");
		exit(0);
	}
	moves->head = init_move(j, i);
	moves->tail = moves->head;
	return moves;
}

void free_movesList(struct MovesList* moves){
	/*free moves list - frees all moves in this list*/
	struct Move *currentMove;
	while (moves->head != moves->tail){
		currentMove = moves->head;
		moves->head = moves->head->next;
		free(currentMove);
	}
	free(moves->head);
}


char *piece_charToString(char c) {
	/*return string representation for a char piece*/
	if (c == BLACK_QUEEN || c == WHITE_QUEEN) {
		return "queen";
	}
	else if (c == BLACK_BISHOP || c == WHITE_BISHOP){
		return "Bishop";
	}
	else if (c == BLACK_KNIGHT || c == WHITE_KNIGHT) {
		return "Knight";
	}
	else if (c == BLACK_ROOK || c == WHITE_ROOK) {
		return "Rook";
	}
	return "d";
}

char piece_stringToChar(char *piece, char *userColor){
	/*return char representation for a string piece*/
	int color = strcmp(userColor, "white");
	if (strcmp(piece, "queen\n") == 0){
		if (color == 0) return WHITE_QUEEN;
		else return BLACK_QUEEN;
	}
	else if (strcmp(piece, "bishop\n") == 0){
		if (color == 0) return WHITE_BISHOP;
		else return BLACK_BISHOP;
	}
	else if (strcmp(piece, "rook\n") == 0){
		if (color == 0) return WHITE_ROOK;
		else return BLACK_ROOK;
	}
	else if (strcmp(piece, "knight\n") == 0){
		if (color == 0) return WHITE_KNIGHT;
		else return BLACK_KNIGHT;
	}
	else return 'd';
}

void print_Moves(struct MovesList* moves){
	/*prints all the moves in the moves list*/
	struct Move *currentMove;
	currentMove = moves->head;
	if (currentMove == moves->tail){
		return;
	}
	currentMove = currentMove->next;
	while (currentMove != moves->tail){/*call for print on each move*/
		if (printf("<%c,%d> ", moves->head->col + 97, moves->head->row + 1) < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		if (printf("to <%c,%d>", currentMove->col + 97, currentMove->row + 1) < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		if (currentMove->changeTo != 'd') {
			if (printf(" %s\n", piece_charToString(currentMove->changeTo)) < 0) {
				perror("Error: standard function printf has failed");
				exit(0);
			}
		}
		else {
			if (printf("%s", "\n") < 0) {
				perror("Error: standard function printf has failed");
				exit(0);
			}
		}
		currentMove = currentMove->next;
	}
	if (printf("<%c,%d> ", moves->head->col + 97, moves->head->row + 1) < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
	if (printf("to <%c,%d>", currentMove->col + 97, currentMove->row + 1) < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
	if (currentMove->changeTo != 'd') {
		if (printf(" %s\n", piece_charToString(currentMove->changeTo)) < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
	}
	else {
		if (printf("%s", "\n") < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
	}
}

void print_userMoves(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char* color){
	/*print possible moves for the user's selected piece*/
	struct MovesList *moves;
	if (isLegal(j, i) == 1){
		if (verifyColor(board, j, i, color) == 1){/*for each of the color's pawns*/
			moves = init_movesList(j, i);
			getMoves_Single(board, moves, j, i);
			clear_kingThreatenningMoves(board, moves);
			print_Moves(moves);
			free_movesList(moves);
			free(moves);
		}
		else{
			print_message(NO_DICS);
		}
	}
	else{
		print_message(WRONG_POSITION);
	}
}

void clear_kingThreatenningMoves(char board[BOARD_SIZE][BOARD_SIZE], MovesList *userMoves){
	/*erase illegal moves that threatens the user's king*/
	char *enemyCol;
	Move *new_currentMove;
	Move *currentMove;
	Move *prevMove;
	char new_board[BOARD_SIZE][BOARD_SIZE];
	if (verifyColor(board, userMoves->head->col, userMoves->head->row, "white") == 1){
		enemyCol = "black";
	}
	else{
		enemyCol = "white";
	}
	prevMove = userMoves->head;/*go over each move and check for chess or checkmate by the enemy*/
	for (currentMove = userMoves->head->next; currentMove != userMoves->tail->next; currentMove = new_currentMove){
		changeBoard_SingleMove(board, new_board, userMoves->head->col, userMoves->head->row, currentMove);
		if (checkForChess(new_board, enemyCol) == 1){
			if (currentMove == userMoves->tail){
				new_currentMove = currentMove->next;
				userMoves->tail = prevMove;
				free(currentMove);
				break;
			}
			prevMove->next = currentMove->next;
			new_currentMove = currentMove->next;
			free(currentMove);
		}
		else{
			new_currentMove = currentMove->next;
			prevMove = currentMove;
		}

	}
}

int isLegal(int j, int i) {
	/*check if this is a valid place on the board*/
	if (i<0 || i>BOARD_SIZE - 1 || j<0 || j>BOARD_SIZE - 1){
		return 0;
	}
	else{
		return 1;
	}
}

int verifyColor(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char *color){
	/*checks if the piece in this index is from the desired color*/
	if (board[j][i] == EMPTY){
		return 0;
	}
	else{
		if (strcmp(color, "black") == 0){
			if (board[j][i] == BLACK_BISHOP || board[j][i] == BLACK_KING || board[j][i] == BLACK_KNIGHT || board[j][i] == BLACK_PAWN || board[j][i] == BLACK_QUEEN || board[j][i] == BLACK_ROOK){
				return 1;
			}
			else{
				return 0;
			}
		}
		else{
			if (board[j][i] == WHITE_BISHOP || board[j][i] == WHITE_KING || board[j][i] == WHITE_KNIGHT || board[j][i] == WHITE_PAWN || board[j][i] == WHITE_QUEEN || board[j][i] == WHITE_ROOK){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
}

void getMoves_pawn_byDelta(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i, int j_Delta, int i_Delta){
	if (isLegal(j + j_Delta, i + i_Delta)){
		if (verifyColor(board, j, i, "black") == 1){
			if ((verifyColor(board, j + j_Delta, i + i_Delta, "white") == 1 && j_Delta != 0) || (board[j + j_Delta][i + i_Delta] == EMPTY && j_Delta == 0)){
				if (i == 1){
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = BLACK_QUEEN;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = BLACK_BISHOP;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = BLACK_ROOK;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = BLACK_KNIGHT;
					moves->tail = moves->tail->next;
				}
				else{
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail = moves->tail->next;
				}
			}
		}
		else if (verifyColor(board, j, i, "white") == 1){
			if ((verifyColor(board, j + j_Delta, i + i_Delta, "black") == 1 && j_Delta != 0) || (board[j + j_Delta][i + i_Delta] == EMPTY && j_Delta == 0)){
				if (i == 6){
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = WHITE_QUEEN;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = WHITE_BISHOP;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = WHITE_ROOK;
					moves->tail = moves->tail->next;
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail->next->changeTo = WHITE_KNIGHT;
					moves->tail = moves->tail->next;
				}
				else{
					moves->tail->next = init_move(j + j_Delta, i + i_Delta);
					moves->tail = moves->tail->next;
				}
			}
		}
	}
}

void getMoves_Pawn(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i) {
	if (board[j][i] == BLACK_PAWN) {
		getMoves_pawn_byDelta(board, moves, j, i, 0, -1);
		getMoves_pawn_byDelta(board, moves, j, i, 1, -1);
		getMoves_pawn_byDelta(board, moves, j, i, -1, -1);
	}
	else if (board[j][i] == WHITE_PAWN) {
		getMoves_pawn_byDelta(board, moves, j, i, 0, 1);
		getMoves_pawn_byDelta(board, moves, j, i, 1, 1);
		getMoves_pawn_byDelta(board, moves, j, i, -1, 1);
	}

}

void getMoves_byDelta(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i, int j_delta, int i_delta){
	int orig_i = i;
	int orig_j = j;
	char *color;
	if (verifyColor(board, orig_j, orig_i, "white") == 1){
		color = "white";
	}
	else{
		color = "black";
	}
	while (isLegal(j + j_delta, i + i_delta) == 1){
		if (verifyColor(board, j + j_delta, i + i_delta, color) == 0){/*empty or enemy square*/
			moves->tail->next = init_move(j + j_delta, i + i_delta);
			moves->tail = moves->tail->next;
			if (board[orig_j][orig_i] == BLACK_KNIGHT || board[orig_j][orig_i] == WHITE_KNIGHT || board[orig_j][orig_i] == BLACK_KING || board[orig_j][orig_i] == WHITE_KING){
				break;
			}
			else{
				if (board[j + j_delta][i + i_delta] != EMPTY){/*if we ate an opponent pawn*/
					break;
				}
				else{/*if no eating was made in that square - go on cheacking*/
					j += j_delta;
					i += i_delta;
				}
			}
		}
		else{/*if we are blocked*/
			break;
		}
	}
}

void getMoves_Rook(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i){
	getMoves_byDelta(board, moves, j, i, -1, 0);
	getMoves_byDelta(board, moves, j, i, 1, 0);
	getMoves_byDelta(board, moves, j, i, 0, -1);
	getMoves_byDelta(board, moves, j, i, 0, 1);
}

void getMoves_Bishop(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i) {
	getMoves_byDelta(board, moves, j, i, 1, 1);
	getMoves_byDelta(board, moves, j, i, 1, -1);
	getMoves_byDelta(board, moves, j, i, -1, 1);
	getMoves_byDelta(board, moves, j, i, -1, -1);
}

void getMoves_Knight(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i){
	getMoves_byDelta(board, moves, j, i, -1, -2);
	getMoves_byDelta(board, moves, j, i, -1, 2);
	getMoves_byDelta(board, moves, j, i, 1, -2);
	getMoves_byDelta(board, moves, j, i, 1, 2);
	getMoves_byDelta(board, moves, j, i, -2, -1);
	getMoves_byDelta(board, moves, j, i, -2, 1);
	getMoves_byDelta(board, moves, j, i, 2, -1);
	getMoves_byDelta(board, moves, j, i, 2, 1);
}

void getMoves_Queen(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i) {
	getMoves_Bishop(board, moves, j, i);
	getMoves_Rook(board, moves, j, i);
}

void getMoves_King(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i){
	getMoves_Queen(board, moves, j, i);
}

void getMoves_Single(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList* moves, int j, int i){
	/*get all the moves for the pieace in this index*/
	moves->head->col = j;
	moves->head->row = i;
	if (board[j][i] == BLACK_KING || board[j][i] == WHITE_KING){
		getMoves_King(board, moves, j, i);
	}
	else if (board[j][i] == BLACK_QUEEN || board[j][i] == WHITE_QUEEN){
		getMoves_Queen(board, moves, j, i);
	}
	else if (board[j][i] == BLACK_KNIGHT || board[j][i] == WHITE_KNIGHT){
		getMoves_Knight(board, moves, j, i);
	}
	else if (board[j][i] == BLACK_BISHOP || board[j][i] == WHITE_BISHOP){
		getMoves_Bishop(board, moves, j, i);
	}
	else if (board[j][i] == BLACK_ROOK || board[j][i] == WHITE_ROOK){
		getMoves_Rook(board, moves, j, i);
	}
	else{
		getMoves_Pawn(board, moves, j, i);
	}
}

int isValidMove(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList *userMove, char *color, int console){
	/*check if the user's move recieved is a valid move*/
	struct MovesList *moves = init_movesList(-1, -1);
	struct Move *curr;
	int legal = 0;
	getMoves_Single(board, moves, userMove->head->col, userMove->head->row);
	if (verifyColor(board, userMove->head->col, userMove->head->row, color) == 0){/*no disc from this color in this position*/
		if (console == 1){
			print_message(NO_DICS);
		}
		free_movesList(moves);
		free(moves);
		return 0;
	}
	if (userMove->head->next == NULL){
		if (console == 1){
			print_message(ILLEGAL_MOVE);
		}
		free_movesList(moves);
		free(moves);
		return 0;
	}
	for (curr = moves->head->next; curr != (moves->tail)->next; curr = curr->next) {/*go over each possible move*/
		if (moves->head->col == userMove->head->col && moves->head->row == userMove->head->row){/*check start square*/
			if (curr->col == userMove->tail->col && curr->row == userMove->tail->row)/*check destination*/
			{
				if (curr->changeTo == userMove->tail->changeTo){
					legal = 1;
					break;
				}
			}
		}
	}
	if (legal == 1){/*check if the move leads to chess or checkmate by the enemy*/
		clear_kingThreatenningMoves(board, userMove);
		if (userMove->head == userMove->tail){
			if (console == 1){
				print_message(ILLEGAL_MOVE);
			}
			free_movesList(moves);
			free(moves);
			return 0;
		}
		else{
			free_movesList(moves);
			free(moves);
			return 1;
		}
	}
	else{
		if (console == 1){
			print_message(ILLEGAL_MOVE);
		}
		free_movesList(moves);
		free(moves);
		return 0;
	}
}

int create_userMove(char board[BOARD_SIZE][BOARD_SIZE], struct MovesList *move, char* color){
	/*create the user's move by the input recieved*/
	char user_input[50];
	char *piece;
	int row = -1;
	int col = -1;
	int i;
	fgets(user_input, sizeof(user_input), stdin);
	for (i = 0; i < 10; i += 9){
		col = (int)(user_input[i + 2] - 'a');
		row = (int)(user_input[i + 4] - '0') - 1;
		if (isLegal(col, row) == 0){/*check location*/
			print_message(WRONG_POSITION);
			return 0;
		}
		else {/*valid location - create move*/
			if (i == 0){
				move->head->col = col;
				move->head->row = row;
			}
			else{
				move->head->next = init_move(col, row);
				move->tail = move->head->next;
				if ((board[move->head->col][move->head->row] == BLACK_PAWN && move->tail->row == 0) || (board[move->head->col][move->head->row] == WHITE_PAWN && move->tail->row == 7)){
					piece = &(user_input[15]);
					if (strcmp(piece, "\n") != 0){
						piece = &(user_input[16]);
						move->tail->changeTo = piece_stringToChar(piece, color);
					}
					else{
						if (verifyColor(board, move->head->col, move->head->row, "white") == 1) { move->tail->changeTo = WHITE_QUEEN; }
						else { move->tail->changeTo = BLACK_QUEEN; }
					}
				}
			}
		}
	}
	return 1;
}

void changeBoard_SingleMove(char board[BOARD_SIZE][BOARD_SIZE], char newBoard[BOARD_SIZE][BOARD_SIZE], int from_col, int from_row, struct Move *destination){
	/*change the game board for a single step*/
	int i, j;
	char *piece;
	int to_col = destination->col;
	int to_row = destination->row;
	for (j = 0; j < BOARD_SIZE; ++j) {
		for (i = 0; i < BOARD_SIZE; ++i) {
			newBoard[j][i] = board[j][i];
		}
	}
	piece = piece_charToString(destination->changeTo);
	if (strcmp(piece, "d") != 0){/*if the pawn changes to another piece*/
		newBoard[to_col][to_row] = destination->changeTo;
	}
	else{
		newBoard[to_col][to_row] = board[from_col][from_row];
	}
	newBoard[from_col][from_row] = EMPTY;
}
