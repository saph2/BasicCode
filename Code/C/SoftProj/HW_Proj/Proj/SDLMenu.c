#include "SDLMenu.h"

int create_main_window(){
	/*build the main window*/
	SDL_Surface *main_window = NULL;
	if (control_Constractor(WINDOW, &main_window, 0, 0, NULL, 0, 0, 0) != 0){/*build window*/
		return 1;
	}
	if (control_Constractor(PANEL, &main_window, 3 * SQUARE, 1 * SQUARE, "Controls/main_panel.bmp", 0, 0, 1) != 0){/*add main panel*/
		SDL_FreeSurface(main_window);
		return 1;
	}
	SDL_FreeSurface(main_window);
	return 0;
}

int create_game_mode_window(){
	/*build game mode selection window*/
	SDL_Surface *game_mode_win = NULL;
	if (control_Constractor(WINDOW, &game_mode_win, 0, 0, NULL, 0, 0, 0) != 0){/*build window*/
		return 1;
	}
	if (control_Constractor(PANEL, &game_mode_win, 3 * SQUARE, 1 * SQUARE, "Controls/game_mode.bmp", 0, 0, 1) != 0){/*build window*/
		SDL_FreeSurface(game_mode_win);
		return 1;
	}
	SDL_FreeSurface(game_mode_win);
	return 0;
}

int valid_square_color(int j, int i, char *color){
	/*checks the square color - returns 1 for true*/
	if (strcmp(color, "black") == 0){
		if (((i + j) % 2) == 0) return 1;
		else return 0;
	}
	else{
		if (((i + j) % 2) == 0) return 0;
		else return 1;
	}
}

char * get_picture(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	/*return the piece picture address*/
	int color = valid_square_color(j, i, "white");
	switch (board[j][i]){
	case WHITE_KING:
		if (color == 1) return ("GameBoard/WKonwhite.bmp");
		else return ("GameBoard/WKonblack.bmp");
		break;
	case BLACK_KING:
		if (color == 1) return ("GameBoard/BKonwhite.bmp");
		else return ("GameBoard/BKonblack.bmp");
		break;
	case WHITE_QUEEN:
		if (color == 1) return ("GameBoard/WQonwhite.bmp");
		else return ("GameBoard/WQonblack.bmp");
		break;
	case BLACK_QUEEN:
		if (color == 1) return ("GameBoard/BQonwhite.bmp");
		else return ("GameBoard/BQonblack.bmp");
		break;
	case WHITE_BISHOP:
		if (color == 1) return ("GameBoard/WBonwhite.bmp");
		else return ("GameBoard/WBonblack.bmp");
		break;
	case BLACK_BISHOP:
		if (color == 1) return ("GameBoard/BBonwhite.bmp");
		else return ("GameBoard/BBonblack.bmp");
		break;
	case WHITE_ROOK:
		if (color == 1) return ("GameBoard/WRonwhite.bmp");
		else return ("GameBoard/WRonblack.bmp");
		break;
	case BLACK_ROOK:
		if (color == 1) return ("GameBoard/BRonwhite.bmp");
		else return ("GameBoard/BRonblack.bmp");
		break;
	case WHITE_KNIGHT:
		if (color == 1) return ("GameBoard/WNonwhite.bmp");
		else return ("GameBoard/WNonblack.bmp");
		break;
	case BLACK_KNIGHT:
		if (color == 1) return ("GameBoard/BNonwhite.bmp");
		else return ("GameBoard/BNonblack.bmp");
		break;
	case WHITE_PAWN:
		if (color == 1) return ("GameBoard/WPonwhite.bmp");
		else return ("GameBoard/WPonblack.bmp");
		break;
	case BLACK_PAWN:
		if (color == 1) return ("GameBoard/BPonwhite.bmp");
		else return ("GameBoard/BPonblack.bmp");
		break;
	case EMPTY:
		if (color == 1) return ("GameBoard/EMPTYwhite.bmp");
		else return ("GameBoard/EMPTYblack.bmp");
		break;
	}
	return NULL;
}

int present_board(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window){
	/*present the board from the consol in sdl*/
	int j, i;
	int col = 0;
	int row = 0;
	char *temp_pic;
	if (control_Constractor(PANEL, &game_window, 0, 0, "GameBoard/board.bmp", 0, 0, 0) != 0){/*build window*/
		return 1;
	}
	for (j = 0; j < BOARD_SIZE; j++){
		for (i = 0; i < BOARD_SIZE; i++){
			temp_pic = get_picture(board, j, i);
			col = j * SQUARE;
			row = (7 - i) * SQUARE;
			if (temp_pic != NULL){/*place pieces on the board*/
				if (col == 7 * SQUARE && row == 0){/*show only after all the pieces are on the board*/
					if (control_Constractor(PANEL, &game_window, col, row, temp_pic, 0, 0, 1) != 0){
						return 1;
					}
				}
				else{
					if (control_Constractor(PANEL, &game_window, col, row, temp_pic, 0, 0, 0) != 0){
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int get_col_Index(int spot){
	/*gets the prressed location and returns the col index*/
	int sq;
	sq = (spot / (SQUARE + 1));
	return sq;
}

int get_row_Index(int spot){
	/*gets the prressed location and returns the row index*/
	int sq;
	sq = 7 - (spot / (SQUARE + 1));
	return sq;
}

int get_col_Square(int spot){
	/*gets the prressed index and returns the square's col*/
	int index;
	index = spot * SQUARE;
	return index;
}

int get_row_Square(int spot){
	/*gets the prressed index and returns the square's row*/
	int index;
	index = (7 - spot) * SQUARE;
	return index;
}

int on_the_board(int j, int i){
	/*return 1 if we are on the game board*/
	SDL_Rect rect = { 0, 0, 8 * SQUARE, 8 * SQUARE };
	if ((j >= rect.x) && (j <= rect.x + rect.w) && (i >= rect.y) && (i <= rect.y + rect.h)){
		return 1;
	}
	return 0;
}

int game_on_sdl() {
	/*open main manu - start GUI game*/
	SDL_Event e;
	char board[BOARD_SIZE][BOARD_SIZE];
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	if (create_main_window() != 0){ /*open main menu*/
		return 1;
	}

	init_board_(board);/*restart static variables*/

	while (!quit) {

		if (SDL_PollEvent(&e) != 0) { /* Poll for keyboard & mouse events*/
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_KEYDOWN) :
				switch (e.key.keysym.sym){
				case SDLK_UP:
					break;
				default: break;
			}
			case (SDL_MOUSEBUTTONUP) :
				/*choosing to start the game*/
				if (e.button.x > SQUARE * 5 && e.button.x < SQUARE * 9 && e.button.y>SQUARE * 3 && e.button.y < SQUARE * 4){

					if (create_game_mode_window() != 0){/*start game - go to game mode window*/
						return 1;
					}

					if (choose_game_mode(board) != 0){/*choose game mode*/
						return 1;
					}
				}
				else if (e.button.x > SQUARE * 5 && e.button.x < SQUARE * 9 && e.button.y>SQUARE * 5 && e.button.y < SQUARE * 6){
					return (save_load_game(LOAD_GAME, board));/*load game*/
				}
				else if (e.button.x > SQUARE * 5 && e.button.x < SQUARE * 9 && e.button.y>SQUARE * 7 && e.button.y < SQUARE * 8){
					exit(0);/*quit*/
				}
				break;
			default: break;
			}
		}
	}
	return 0;
}

int game_panels(char board[BOARD_SIZE][BOARD_SIZE], SDL_Surface *game_window, int row, int *game_mode, int *current_turn, int *user_color, int *difficulty, int *game_over, int *best){
	/*selection on the game panels*/
	if (row > 0 && row < 1 * SQUARE){/*restart current game*/
		init_board_(board);
		SDL_FreeSurface(game_window);
		*current_turn = 0;
		if (*game_mode == 1){
			*user_color = *current_turn;
		}
		return start_game(board);
	}
	else if (row > 2 * SQUARE && row < 3 * SQUARE){/*save the current game*/
		SDL_FreeSurface(game_window);
		return save_load_game(SAVE_GAME, board);
	}
	else if (row >4 * SQUARE && row < 5 * SQUARE){/*return to main menu*/
		SDL_FreeSurface(game_window);
		restartValues(game_mode, current_turn, user_color, difficulty, game_over, best);
		return game_on_sdl();
	}
	else if (row>6 * SQUARE && row < 7 * SQUARE){/*quit*/
		SDL_FreeSurface(game_window);
		exit(0);
	}
	return 0;
}

