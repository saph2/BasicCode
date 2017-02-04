#include "SDLGame.h"

static int current_turn = 0;
static int step_count = 0;
static int get_moves_mode = 1;
static int difficulty = 1;
static int user_color = 0;
static int game_mode = 0;
static int game_over = 0;
static int best = 0;

void restartValues(int *gameMode, int *currentTurn, int *userColor, int *diff, int *gameOver, int *bestDiff){
	/*reset static values for a new game*/
	*currentTurn = 0;
	*diff = 1;
	*userColor = 0;
	*gameMode = 1;
	*gameOver = 0;
	*bestDiff = 0;
}

int show_possible_moves(int j, int i, char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window){
	/*show possible moves for piece on the game board*/
	struct MovesList *moves = init_movesList(j, i);
	struct Move *curr;
	int col, row;
	getMoves_Single(board, moves, j, i);/*get moves for index*/
	clear_kingThreatenningMoves(board, moves);/*clear illegal moves*/
	if (moves->head->next != NULL){
		for (curr = moves->head->next; curr != (moves->tail)->next; curr = curr->next) {/*mark each possible move*/
			col = get_col_Square(curr->col);
			row = get_row_Square(curr->row);
			if (curr->next == (moves->tail)->next){
				if (control_Constractor(PANEL, &game_window, col, row, "GameBoard/movesSquare.bmp", 1, 100, 1) != 0){
					free_movesList(moves);
					free(moves);
					return 1;
				}
			}
			else{
				if (control_Constractor(PANEL, &game_window, col, row, "GameBoard/movesSquare.bmp", 1, 100, 0) != 0){
					free_movesList(moves);
					free(moves);
					return 1;
				}
			}
		}
	}
	else{/*no possible moves*/
		current_turn = 0;
	}
	free_movesList(moves);
	free(moves);
	return 0;
}


int mark_selected(int j, int i, char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window, char *color){
	/*mark the SQUARE chosen by the user and send for 'show possible moves'*/
	int sq_col = -1;
	int sq_row = -1;
	if (step_count < 1){
		if (on_the_board(j, i) == 1 && verifyColor(board, j, i, color) == 1){/*if we pressed our piece*/
			sq_col = get_col_Square(j);
			sq_row = get_row_Square(i);
			step_count++;
			if (control_Constractor(PANEL, &game_window, sq_col, sq_row, "GameBoard/chosenSquare.bmp", 1, 85, 1) != 0){
				return 1;
			}
			if (get_moves_mode == 1 && step_count == 1){/*call for the piece possible moves*/
				if (show_possible_moves(j, i, board, game_window) != 0){
					return 1;
				}
			}
		}
	}
	else{
		step_count = 0;/*clean the board from all selections*/
		return (present_board(board, game_window));
	}
	return 0;
}

int choose_new_piece(char board[BOARD_SIZE][BOARD_SIZE], MovesList *userMove, int col, int row, SDL_Surface *game_window, char *color) {
	/*user selection for alternative piece insted of his pawn*/
	int quit = 0;
	SDL_Event e;
	if (strcmp(color, "black") == 0) {/*selection panel by color*/
		if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/choose_black.bmp", 0, 0, 1) != 0){
			return 1;
		}
	}
	else{
		if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/choose_white.bmp", 0, 0, 1) != 0){
			return 1;
		}
	}

	while (!quit) {
		if (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :/*new piece selection loop*/
				if (e.button.x > 2 * SQUARE && e.button.x < 3 * SQUARE && e.button.y > 8 * SQUARE && e.button.y <9 * SQUARE){
					if (strcmp(color, "black") == 0){
						userMove->tail->changeTo = BLACK_ROOK;
					}
					else {
						userMove->tail->changeTo = WHITE_ROOK;
					}
					return 0;
				}
				else if (e.button.x > 3 * SQUARE && e.button.x < 4 * SQUARE && e.button.y > 8 * SQUARE && e.button.y < 9 * SQUARE){
					if (strcmp(color, "black") == 0){
						userMove->tail->changeTo = BLACK_KNIGHT;
					}
					else {
						userMove->tail->changeTo = WHITE_KNIGHT;
					}
					return 0;
				}
				else if (e.button.x > 4 * SQUARE && e.button.x < 5 * SQUARE && e.button.y > 8 * SQUARE && e.button.y < 9 * SQUARE){
					if (strcmp(color, "black") == 0){
						userMove->tail->changeTo = BLACK_BISHOP;
					}
					else {
						userMove->tail->changeTo = WHITE_BISHOP;
					}
					return 0;
				}
				else if (e.button.x > 5 * SQUARE && e.button.x < 6 * SQUARE && e.button.y > 8 * SQUARE && e.button.y < 9 * SQUARE){
					if (strcmp(color, "black") == 0){
						userMove->tail->changeTo = BLACK_QUEEN;
					}
					else {
						userMove->tail->changeTo = WHITE_QUEEN;
					}
					return 0;
				}
				break;
			default: break;
			}
		}
	}
	return quit;
}

int make_move(char board[BOARD_SIZE][BOARD_SIZE], MovesList **userMove, int col, int row, SDL_Surface *game_window, char *color){
	/*make a move on the board by the user's selection*/

	if (current_turn == 0){/*choose piece to move*/
		if (isLegal(col, row) == 1){/*legal Square*/
			if (verifyColor(board, col, row, color) == 1){/*user color was selected*/
				current_turn++;
				(*userMove)->head->col = col;
				(*userMove)->head->row = row;
			}
			else{
				current_turn = 0;
				step_count = 0;
				free_movesList(*userMove);
				free(*userMove);
			}
		}
		else{
			current_turn = 0;
			step_count = 0;
			free_movesList(*userMove);
			free(*userMove);
		}
	}
	else if (current_turn == 1) {/*choose location to move piece to*/
		if (isLegal(col, row) == 1){/*legal Square*/
			current_turn++;
			(*userMove)->head->next = init_move(col, row);
			(*userMove)->tail = (*userMove)->head->next;
			if ((board[(*userMove)->head->col][(*userMove)->head->row] == BLACK_PAWN && (*userMove)->tail->row == 0) || (board[(*userMove)->head->col][(*userMove)->head->row] == WHITE_PAWN && (*userMove)->tail->row == 7)){/*pawn selection*/
				if (strcmp(color, "black") == 0) {
					(*userMove)->tail->changeTo = BLACK_QUEEN;
				}
				else {
					(*userMove)->tail->changeTo = WHITE_QUEEN;
				}
			}
			if (isValidMove(board, (*userMove), color,0) == 1){/*valid move*/
				if ((board[(*userMove)->head->col][(*userMove)->head->row] == BLACK_PAWN && (*userMove)->tail->row == 0) || (board[(*userMove)->head->col][(*userMove)->head->row] == WHITE_PAWN && (*userMove)->tail->row == 7)){
					if (choose_new_piece(board, (*userMove), (*userMove)->tail->col, (*userMove)->tail->row, game_window, color) != 0){
						free_movesList(*userMove);
						free(*userMove);
						return 1;
					}
				}
				changeBoard_SingleMove(board, board, (*userMove)->head->col, (*userMove)->head->row, (*userMove)->tail);/*make move*/
				free_movesList(*userMove);
				free(*userMove);

				if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/del.bmp", 0, 0, 1) != 0){
					return 1;
				}

				if (present_board(board, game_window) != 0){/*present new board*/
					printf("ERROR: failed to present board: %s\n", SDL_GetError());
					return 1;
				}

				if (checkForCheckmate(board, color) == 1){ /*check for win*/
					game_over = 1;/*if checkmate - game is over*/
					if (strcmp(color, "white") == 0){
						if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/white_wins.bmp", 0, 0, 1) != 0){
							return 1;
						}
					}
					else{
						if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/black_wins.bmp", 0, 0, 1) != 0){
							return 1;
						}
					}
				}
				else if (checkForChess(board, color) == 1){/*check for chess*/
					if (strcmp(color, "white") == 0){
						if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_black.bmp", 0, 0, 1) != 0){
							return 1;
						}
					}
					else{
						if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_white.bmp", 0, 0, 1) != 0){
							return 1;
						}
					}
				}
				else{
					if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/del.bmp", 0, 0, 1) != 0){
						return 1;
					}
				}
			}
			else{
				step_count = 0;
				current_turn = 0;
				free_movesList(*userMove);
				free(*userMove);
			}
		}
		else{
			step_count = 0;
			current_turn = 0;
			free_movesList(*userMove);
			free(*userMove);
		}
	}
	return 0;
}

int choose_difficulty(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_setting_win, int j, int i){
	/*choose difficulty  - minimax depth*/
	int col, row;

	if (control_Constractor(PANEL, &game_setting_win, 5 * SQUARE, 6 * SQUARE, "Controls/choose_diff.bmp", 0, 0, 0) != 0){
		return 1;
	}

	col = get_col_Square(j);
	row = get_row_Square(i);

	if (control_Constractor(PANEL, &game_setting_win, col, row, "Controls/markDif.bmp", 1, 85, 1) != 0){
		return 1;
	}

	switch (j){/*update difficulty*/
	case 5: difficulty = 1; break;
	case 6: difficulty = 2; break;
	case 7: difficulty = 3; break;
	case 8: difficulty = 4; break;
	case 9: /*best*/ best = 1; break;
	default: difficulty = 1; break;
	}
	return 0;
}

int choose_color(SDL_Surface *game_setting_win, int j, int i){
	/*choose user color*/
	int col, row;

	if (control_Constractor(PANEL, &game_setting_win, 5 * SQUARE, 4 * SQUARE, "Controls/choose_color.bmp", 0, 0, 0) != 0){
		return 1;
	}

	if (j == 6){
		j = 5;
	}
	else if (j == 8){
		j = 7;
	}
	if (j == 7){
		user_color = 1;
		current_turn = 2;
	}
	else {
		user_color = 0;
		current_turn = 0;
	}

	col = get_col_Square(j);
	row = get_row_Square(i);

	if (control_Constractor(PANEL, &game_setting_win, col, row, "Controls/markCol.bmp", 1, 85, 1) != 0){
		return 1;
	}

	return 0;
}

int game_on_AI(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window){
	/*play game - Player vs. AI mode*/
	MovesList *compBestMove;
	MovesList *userMove;
	char *computerColor;
	char *playersColor;
	int col, row;
	SDL_Event e;
	int quit = 0;

	if (user_color == 0){
		playersColor = "white";
		computerColor = "black";
	}
	else{
		playersColor = "black";
		computerColor = "white";
	}

	if (current_turn == 1){/*restart mouse presses*/
		current_turn = 0;
	}

	while (!quit){

		if (current_turn == 0){
			if (control_Constractor(PANEL, &game_window, 9 * SQUARE - 15, 8 * SQUARE, "Controls/turn_player.bmp", 0, 0, 1) != 0){/*applay the game panels*/
				SDL_FreeSurface(game_window);
				return 1;
			}
		}
		else if (current_turn == 2){
			if (control_Constractor(PANEL, &game_window, 9 * SQUARE - 15, 8 * SQUARE, "Controls/turn_computer.bmp", 0, 0, 1) != 0){/*applay the game panels*/
				SDL_FreeSurface(game_window);
				return 1;
			}
		}

		if (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				if (current_turn < 2){/*user's turn*/
					if (e.button.x>8 * SQUARE && e.button.x<12 * SQUARE && e.button.y>0 && e.button.y < 8 * SQUARE){/*game panels selected*/
						if (current_turn == 1){
							free_movesList(userMove);
							free(userMove);
						}
						if (game_panels(board, game_window, e.button.y, &game_mode, &current_turn, &user_color, &difficulty, &game_over, &best) != 0){/*game panels selection*/
							return 1;
						}
						break;
					}
					else if (game_over == 0){/*while the game is still on*/
						if (current_turn == 0){
							userMove = init_movesList(-1, -1);
						}
						col = get_col_Index(e.button.x);
						row = get_row_Index(e.button.y);
						if (mark_selected(col, row, board, game_window, playersColor) != 0){/*selecet Square*/
							free_movesList(userMove);
							free(userMove);
							return 1;
						}
						if (make_move(board, &userMove, col, row, game_window, playersColor) != 0){/*make a move*/
							return 1;
						}
					}
				}
				break;
			default: break;
			}
			SDL_Delay(15);
		}
		else if (current_turn == 2 && game_over == 0){/*computer's turn*/
			current_turn = 0;
			step_count = 0;
			col = -1;
			row = -1;
			compBestMove = init_movesList(-1, -1);
			compBestMove->head->next = init_move(-1, -1);
			compBestMove->tail = compBestMove->head->next;
			if (best == 1) {
				difficulty = 4;
			}
			alphaBetaMax(-10000000, 10000000, difficulty, 0, board, computerColor, compBestMove, best);/*minimax function called*/
			changeBoard_SingleMove(board, board, (compBestMove)->head->col, (compBestMove)->head->row, (compBestMove)->head->next);

			free_movesList(compBestMove);
			free(compBestMove);

			if (present_board(board, game_window) != 0){
				printf("ERROR: failed to present board: %s\n", SDL_GetError());
				return 1;
			}
			if (checkForCheckmate(board, computerColor) == 1){/*check for win*/
				game_over = 1;/*if checkmate - game is over*/
				if (strcmp(computerColor, "white") == 0){
					if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/white_wins.bmp", 0, 0, 1) != 0){
						return 1;
					}
				}
				else{
					if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/black_wins.bmp", 0, 0, 1) != 0){
						return 1;
					}
				}
			}
			else if (checkForChess(board, computerColor) == 1){ /*check for chess*/
				if (strcmp(computerColor, "white") == 0){
					if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_black.bmp", 0, 0, 1) != 0){
						return 1;
					}
				}
				else{
					if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_white.bmp", 0, 0, 1) != 0){
						return 1;
					}
				}
			}
			else{
				if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/del.bmp", 0, 0, 1) != 0){
					return 1;
				}
			}
		}
	}
	return quit;
}

int game_on_TwoPlayer(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window){
	/*play game - Player vs. Player mode*/
	MovesList *userMove = NULL;
	int col, row;
	SDL_Event e;
	int quit = 0;
	char *playersColor;

	if (current_turn == 1){/*restart mouse presses*/
		current_turn = 0;
	}

	while (!quit){

		if (user_color == 0){
			playersColor = "white";
			if (control_Constractor(PANEL, &game_window, 9*SQUARE - 15, 8*SQUARE, "Controls/turn_white.bmp", 0, 0, 1) != 0){/*applay the game panels*/
				SDL_FreeSurface(game_window);
				return 1;
			}
		}
		else{
			playersColor = "black";
			if (control_Constractor(PANEL, &game_window, 9 * SQUARE - 15, 8 * SQUARE, "Controls/turn_black.bmp", 0, 0, 1) != 0){/*applay the game panels*/
				SDL_FreeSurface(game_window);
				return 1;
			}
		}

		if (SDL_PollEvent(&e) != 0) { /* Poll for keyboard & mouse events*/
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				if (current_turn < 2){/*user's turn*/
					if (e.button.x>8 * SQUARE && e.button.x<12 * SQUARE && e.button.y>0 && e.button.y < 8 * SQUARE){/*panel selected*/
						if (current_turn == 1){
							free_movesList(userMove);
							free(userMove);
						}
						if (game_panels(board, game_window, e.button.y, &game_mode, &current_turn, &user_color, &difficulty, &game_over, &best) != 0){/*game panels selection*/
							return 1;
						}
						break;
					}
					else if (game_over == 0){/*while the game is still on*/
						if (current_turn == 0){
							userMove = init_movesList(-1, -1);
						}
						col = get_col_Index(e.button.x);
						row = get_row_Index(e.button.y);

						if (mark_selected(col, row, board, game_window, playersColor) != 0){/*select Square*/
							free_movesList(userMove);
							free(userMove);
							return 1;
						}
						if (make_move(board, &userMove, col, row, game_window, playersColor) != 0){/*make a move*/
							return 1;
						}
					}
				}
				break;
			default: break;
			}
			SDL_Delay(15);
		}
		else if (current_turn == 2){/*switch turn*/
			step_count = 0;
			current_turn = 0;
			user_color = abs(user_color - 1);
		}
	}
	return quit;
}

int start_game(char board[BOARD_SIZE][BOARD_SIZE]){
	/*start the game both 'P vs. P' and 'P vs. AI'*/
	SDL_Surface* game_window = NULL;
	game_over = 0;
	step_count = 0;
	if (control_Constractor(WINDOW, &game_window, 0, 0, NULL, 0, 0, 0) != 0){/*build game window*/
		return 1;
	}

	if (control_Constractor(PANEL, &game_window, 0, 0, "Controls/game_panels.bmp", 0, 0, 0) != 0){/*applay the game panels*/
		SDL_FreeSurface(game_window);
		return 1;
	}

	if (present_board(board, game_window) != 0){/*update board*/
		printf("ERROR: failed to present board: %s\n", SDL_GetError());
		SDL_FreeSurface(game_window);
		return 1;
	}

	if (checkForChess(board, "white") == 1){/*check for chess on board*/
		if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_black.bmp", 0, 0, 1) != 0){
			SDL_FreeSurface(game_window);
			return 1;
		}
	}
	else if (checkForChess(board, "black") == 1){
		if (control_Constractor(PANEL, &game_window, 0, 8 * SQUARE, "Controls/check_on_white.bmp", 0, 0, 1) != 0){
			SDL_FreeSurface(game_window);
			return 1;
		}
	}

	if (game_mode == 1){/*start Player vs. Player game*/
		if (game_on_TwoPlayer(board, game_window) != 0){
			SDL_FreeSurface(game_window);
			return 1;
		}
	}
	if (game_mode == 2){/*start Player vs. AI game*/
		if (game_on_AI(board, game_window) != 0){
			SDL_FreeSurface(game_window);
			return 1;
		}
	}
	SDL_FreeSurface(game_window);
	return 0;
}


int start_setting(char board[BOARD_SIZE][BOARD_SIZE]){
	/*game setting - Player vs. AI set*/
	SDL_Surface *game_setting_win = NULL;
	SDL_Event e;
	int quit = 0;
	int col, row;
	if (control_Constractor(WINDOW, &game_setting_win, 0, 0, NULL, 0, 0, 0) != 0){/*setting window*/
		return 1;
	}

	if (control_Constractor(PANEL, &game_setting_win, 3 * SQUARE, 1 * SQUARE, "Controls/setting_mode.bmp", 0, 0, 1) != 0){/*setting window*/
		SDL_FreeSurface(game_setting_win);
		return 1;
	}

	while (!quit){
		if (SDL_PollEvent(&e) != 0) { /* Poll for keyboard & mouse events*/
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :/*start game*/
				if (e.button.x > 3 * SQUARE && e.button.x < 6 * SQUARE && e.button.y > 7 * SQUARE && e.button.y < 8 * SQUARE){
					if (start_game(board) != 0){
						SDL_FreeSurface(game_setting_win);
						return 1;
					}
				}/*choose difficulty*/
				if (e.button.x>5 * SQUARE && e.button.x<10 * SQUARE && e.button.y > 6 * SQUARE && e.button.y < 7 * SQUARE){
					col = get_col_Index(e.button.x);
					row = get_row_Index(e.button.y);
					if (choose_difficulty(board, game_setting_win, col, row) != 0){
						SDL_FreeSurface(game_setting_win);
						return 1;
					}
				}/*choose color*/
				if (e.button.x>5 * SQUARE && e.button.x< 9 * SQUARE && e.button.y > 4 * SQUARE && e.button.y < 5 * SQUARE){
					col = get_col_Index(e.button.x);
					row = get_row_Index(e.button.y);
					if (choose_color(game_setting_win, col, row) != 0){
						SDL_FreeSurface(game_setting_win);
						return 1;
					}
				}
				break;
			}
		}
	}
	SDL_FreeSurface(game_setting_win);
	return quit;
}

int choose_game_mode(char board[BOARD_SIZE][BOARD_SIZE]){
	/*game mode selection*/
	SDL_Event e;
	int quit = 0;
	while (!quit){
		if (SDL_PollEvent(&e) != 0) { /* Poll for keyboard & mouse events*/
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				if (e.button.x > 5 * SQUARE && e.button.x < 9 * SQUARE && e.button.y > 3 * SQUARE && e.button.y < 4 * SQUARE){
					game_mode = 1;/*start Player vs. Player game*/
					if (start_game(board) != 0){
						return 1;
					}
				}
				if (e.button.x > 5 * SQUARE && e.button.x < 9 * SQUARE && e.button.y > 5 * SQUARE && e.button.y < 6 * SQUARE){
					game_mode = 2;/*start Player vs. AI setting*/
					if (start_setting(board) != 0){
						return 1;
					}
				}
				if (e.button.x > 5 * SQUARE && e.button.x < 9 * SQUARE && e.button.y > 7 * SQUARE && e.button.y < 8 * SQUARE){
					restartValues(&game_mode, &current_turn, &user_color, &difficulty, &game_over, &best);
					return game_on_sdl();/*return to main menu*/
				}
				break;
			}
		}
	}
	return quit;
}

int save_load_game(char save_or_load, char board[BOARD_SIZE][BOARD_SIZE]){
	/*save or load a game*/
	SDL_Surface *save_load_window = NULL;
	SDL_Event e;
	int quit = 0;
	int ok_to_save_load = 0;
	char *game_name;

	if (control_Constractor(WINDOW, &save_load_window, 0, 0, NULL, 0, 0, 0) != 0){
		return 1;
	}
	if (save_or_load == SAVE_GAME){/*save panel*/
		if (control_Constractor(PANEL, &save_load_window, 3 * SQUARE, 1 * SQUARE, "Controls/save_panel.bmp", 0, 0, 1) != 0){
			SDL_FreeSurface(save_load_window);
			return 1;
		}
	}
	else{/*load panel*/
		if (control_Constractor(PANEL, &save_load_window, 3 * SQUARE, 1 * SQUARE, "Controls/load_panel.bmp", 0, 0, 1) != 0){
			SDL_FreeSurface(save_load_window);
			return 1;
		}
	}

	while (!quit){
		if (SDL_PollEvent(&e) != 0) { /* Poll for keyboard & mouse events*/
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :/*choose game name to save to or to load from*/
				if (e.button.x > 4 * SQUARE && e.button.x < 10 * SQUARE && e.button.y > 3 * SQUARE && e.button.y < 4 * SQUARE){
					game_name = "Game_1.xml";
					ok_to_save_load = 1;
				}
				else if (e.button.x > 4 * SQUARE && e.button.x < 10 * SQUARE && e.button.y > 4 * SQUARE && e.button.y < 5 * SQUARE){
					game_name = "Game_2.xml";
					ok_to_save_load = 1;
				}
				else if (e.button.x > 4 * SQUARE && e.button.x < 10 * SQUARE && e.button.y > 5 * SQUARE && e.button.y < 6 * SQUARE){
					game_name = "Game_3.xml";
					ok_to_save_load = 1;
				}
				else if (e.button.x > 4 * SQUARE && e.button.x < 10 * SQUARE && e.button.y > 6 * SQUARE && e.button.y < 7 * SQUARE){
					game_name = "Game_4.xml";
					ok_to_save_load = 1;
				}
				else if (e.button.x > 4 * SQUARE && e.button.x < 10 * SQUARE && e.button.y > 7 * SQUARE && e.button.y < 8 * SQUARE){
					game_name = "Game_5.xml";
					ok_to_save_load = 1;
				}
				break;
			default: break;
			}
			if (ok_to_save_load == 1){
				SDL_FreeSurface(save_load_window);
				if (save_or_load == SAVE_GAME){/*save game*/
					if (save_game(board, game_mode, difficulty, best, user_color, current_turn, game_name) != 0){
						
						return 1;
					}
				}
				else{/*load game*/
					if (load_game(board, &game_mode, &difficulty, &best, &user_color, &current_turn, game_name) != 0){
						return 1;
					}
				}
				return start_game(board);
			}
		}
	}
	SDL_FreeSurface(save_load_window);
	return quit;
}


