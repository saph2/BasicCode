#include "chessprog.h"
#include "Moves.h"
#include "Game.h"
#include "files.h"
#include "SDLControl.h"
#include "SDLMenu.h"
#include "SDLGame.h"


static int current_turn = 0;
static int user_color = 0;
static int min_max_depth = 1;
static int game_mode = 1;
static int best = 0;

void print_line(){
	int i;
	if (printf(" |") < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
	for (i = 1; i < BOARD_SIZE * 4; i++){
		if (printf("-") < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
	}
	if (printf("|\n") < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
}

void print_board_(char board[BOARD_SIZE][BOARD_SIZE])
{ /*print game board*/
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		if (printf("%d", j + 1) < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		for (i = 0; i < BOARD_SIZE; i++){
			if (printf("| %c ", board[i][j])<0) {
				perror("Error: standard function printf has failed");
				exit(0);
			}
		}
		if (printf("|\n")<0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		print_line();
	}
	if (printf("  ")<0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
	for (j = 0; j < BOARD_SIZE; j++){
		if (printf(" %c  ", (char)('a' + j))<0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
	}
	if (printf("\n") < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}
}



void init_board_(char board[BOARD_SIZE][BOARD_SIZE]){
	/*initializes a standard game board*/
	int i, j;
	for (i = 0; i < BOARD_SIZE; ++i) {
		for (j = 0; j < BOARD_SIZE; ++j) {
			board[j][i] = EMPTY;
		}
	}
	board[0][0] = WHITE_ROOK;
	board[1][0] = WHITE_KNIGHT;
	board[2][0] = WHITE_BISHOP;
	board[3][0] = WHITE_QUEEN;
	board[4][0] = WHITE_KING;
	board[5][0] = WHITE_BISHOP;
	board[6][0] = WHITE_KNIGHT;
	board[7][0] = WHITE_ROOK;
	board[0][7] = BLACK_ROOK;
	board[1][7] = BLACK_KNIGHT;
	board[2][7] = BLACK_BISHOP;
	board[3][7] = BLACK_QUEEN;
	board[4][7] = BLACK_KING;
	board[5][7] = BLACK_BISHOP;
	board[6][7] = BLACK_KNIGHT;
	board[7][7] = BLACK_ROOK;
	for (i = 0; i < BOARD_SIZE; ++i) {
		board[i][1] = WHITE_PAWN;
		board[i][6] = BLACK_PAWN;
	}

}

void clear(char board[BOARD_SIZE][BOARD_SIZE]){
	/*clear the board*/
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			board[j][i] = EMPTY;
		}
	}
}

void set(char board[BOARD_SIZE][BOARD_SIZE], int j, int i, char *color, char *type){
	/*set pawn/rook/bishop/knight/queen/king on the board*/
	char s;
	if (strcmp(color, "black") == 0){
		if (strcmp(type, "rook") == 0){
			s = BLACK_ROOK;
		}
		if (strcmp(type, "knight") == 0){
			s = BLACK_KNIGHT;
		}
		if (strcmp(type, "bishop") == 0) {
			s = BLACK_BISHOP;
		}
		if (strcmp(type, "king") == 0) {
			s = BLACK_KING;
		}
		if (strcmp(type, "queen") == 0) {
			s = BLACK_QUEEN;
		}
		if (strcmp(type, "pawn") == 0) {
			s = BLACK_PAWN;
		}
	}
	else{
		if (strcmp(color, "white") == 0){
			if (strcmp(type, "rook") == 0){
				s = WHITE_ROOK;
			}
			if (strcmp(type, "knight") == 0){
				s = WHITE_KNIGHT;
			}
			if (strcmp(type, "bishop") == 0) {
				s = WHITE_BISHOP;
			}
			if (strcmp(type, "king") == 0) {
				s = WHITE_KING;
			}
			if (strcmp(type, "queen") == 0) {
				s = WHITE_QUEEN;
			}
			if (strcmp(type, "pawn") == 0) {
				s = WHITE_PAWN;
			}
		}
	}
	board[j][i] = s;
}

void rm(char board[BOARD_SIZE][BOARD_SIZE], int j, int i){
	if (isLegal(j, i) == 1){
		board[j][i] = EMPTY;
	}
}

int save_game(char board[BOARD_SIZE][BOARD_SIZE], int gameState, int difficulty, int best, int color, int nextTurn, char *file_chosen) {
	/*save the current game*/
	char *file_name;
	int i;
	int char_count = 0;
	char *next_turn;
	char *userColor;
	char user_input[50];
	FILE *xml = NULL;

	if (file_chosen == NULL){
		fgets(user_input, sizeof(user_input), stdin);
		for (i = 1; i < 50; i++){
			if (user_input[i] != '\n') {
				char_count++;
			}
			else{
				break;
			}
		}
		file_name = (&user_input[1]);
		strtok(file_name, "\n");
	}
	else{
		file_name = file_chosen;
	}
	xml = createXML(file_name);
	if (xml == NULL) {
		perror("Wrong file name\n");
	}
	else{
		if (color == 0){
			userColor = "white";
		}
		else{
			userColor = "black";
		}

		next_turn = userColor;

		write_game_settings(xml, gameState, difficulty, best, userColor, next_turn);
		write_board(board, xml);
		fclose(xml);
		return 0;
	}
	return 1;
}


char* load_file_to_memory(char* xml_file) {
	char *source = NULL;
	size_t newLen;
	FILE *fp = fopen(xml_file, "r");
	if (fp == NULL) {
		return NULL;
	}
	else{
		if (fseek(fp, 0L, SEEK_END) == 0) {
			int bufsize = ftell(fp);
			if (bufsize == -1) { /* Error */ }
			source = malloc(sizeof(char)* (bufsize + 1));
			if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }
			newLen = fread(source, sizeof(char), bufsize, fp);
			if (newLen == 0) {
				fputs("Error reading file", stderr);
			}
			else {
				source[bufsize] = '\0';
			}
			fclose(fp);
		}
		return source;
	}
}

int load_game(char board[BOARD_SIZE][BOARD_SIZE], int *gameMode, int *difficulty, int *best, int *color, int *currentTurn, char * file_chosen){
	/*load a game from file_name or consol*/
	char *file_name;
	char *fp;
	char user_input[100];
	char *userColor;
	char *turn;
	char *diff;

	if (file_chosen == NULL){
		fgets(user_input, sizeof(user_input), stdin);
		file_name = &(user_input[1]);
		strtok(file_name, "\n");
	}
	else{
		file_name = file_chosen;
	}

	fp = load_file_to_memory(file_name);
	if (fp == NULL){
		perror("Wrong file name\n");
		return 1;
	}
	else{
		*gameMode = load_field(fp, "<type>");
		turn = load_color_turn_difficulty(fp, "<next_turn>");
		load_board(board, fp);
		if (*gameMode == 1){
			if (strcmp(turn, "white") == 0){
				*currentTurn = 0;
			}
			else{
				*currentTurn = 1;
			}
			*color = *currentTurn; /*first players color by the current turn*/
			free(fp);
			return 0;
		}
		else {
			diff = load_color_turn_difficulty(fp, "<difficulty>");
			if (strcmp(diff, "best") == 0){
				*best = 1;
			}
			else{
				*best = 0;
				*difficulty = (int)(*diff - '0');
			}
			userColor = load_color_turn_difficulty(fp, "<user_color>");
			if (strcmp(userColor, "white") == 0){
				*color = 0;
			}
			else{
				*color = 1;
			}
			if (strcmp(turn, "white") == 0 && (*color) == 0){
				*currentTurn = 0;
			}
			else if (strcmp(turn, "black") == 0 && (*color) == 0){
				*currentTurn = 1;
			}
			else if (strcmp(turn, "white") == 0 && (*color) == 1){
				*currentTurn = 1;
			}
			else if (strcmp(turn, "black") == 0 && (*color) == 1){
				*currentTurn = 0;
			}
			free(fp);
			return 0;
		}
		free(fp);
	}
	return 0;
}


char *gameState_twoPlayers(char board[BOARD_SIZE][BOARD_SIZE]){
	/*game state - Player vs. Player*/
	char user_input[50];
	int input;
	char *last_command;
	char *currentColor;
	char *colorPrint = "White";
	struct MovesList *userMove;
	int j, i;
	if (printf("%s player - enter your move:\n", colorPrint) < 0) {
		perror("Error: standard function printf has failed");
		exit(0);
	}/*players first command*/
	if (scanf("%s", user_input) != 1) {
		perror("Error: standard function scanf has failed");
		exit(0);
	}

	last_command = &(*user_input);
	while (strcmp(user_input, "quit") != 0 && strcmp(user_input, "restart") != 0){
		if (user_color == 0){
			currentColor = "white";
		}
		else{
			currentColor = "black";
		}
		if (strcmp(user_input, "move") == 0){/*user enter's a move*/
			userMove = init_movesList(-1, -1);
			if (create_userMove(board, userMove, currentColor) == 0){/*move wasn't created*/
				current_turn = abs(current_turn - 1);
				user_color = abs(user_color - 1);
			}
			else if (isValidMove(board, userMove, currentColor, 1) == 0){/*move created wasn't legal*/
				current_turn = abs(current_turn - 1);
				user_color = abs(user_color - 1);
			}
			else{/*if the move is legal - make the move*/
				changeBoard_SingleMove(board, board, userMove->head->col, userMove->head->row, userMove->tail);
				print_board_(board);
				if (checkForCheckmate(board, currentColor) == 1){/*if user won*/
					if (printf("Mate! %s player wins the game\n", colorPrint) < 0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
					free_movesList(userMove);
					free(userMove);
					break;
				}
				else if (checkForChess(board, currentColor) == 1){/*if enemy is now under check*/
					if (printf("Check!\n") < 0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
				}
			}
			free_movesList(userMove);
			free(userMove);
		}
		else if (strcmp(user_input, "get_moves") == 0){/*the user asked for a piece's possible moves*/
			if (scanf("%s", user_input) != 1) {
				perror("Error: standard function scanf has failed");
				exit(0);
			}
			j = (int)(user_input[1] - 'a');
			i = (int)(user_input[3] - '0') - 1;
			print_userMoves(board, j, i, currentColor);
			current_turn = abs(current_turn - 1);
			user_color = abs(user_color - 1);
		}
		else if (strcmp(user_input, "quit") == 0){/*the user wishes to quit the game*/
			exit(0);
		}
		else if (strcmp(user_input, "save") == 0){/*the user wishes to save the game*/
			save_game(board, 1, min_max_depth, best, user_color, current_turn, NULL);
			current_turn = abs(current_turn - 1);
			user_color = abs(user_color - 1);
		}
		else{
			while ((input = getchar()) != EOF && input != '\n'){
				if (scanf("%s", user_input) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
			}
			print_message(ILLEGAL_COMMAND);
			current_turn = abs(current_turn - 1);
			user_color = abs(user_color - 1);
		}
		current_turn = abs(current_turn - 1);/*switch to next palyer's turn*/
		user_color = abs(user_color - 1);/*switch to next palyer's color*/
		if (user_color == 0){
			colorPrint = "White";
		}
		else{
			colorPrint = "Black";
		}
		if (printf("%s player - enter your move:\n", colorPrint) < 0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		if (scanf("%s", user_input) != 1) {
			perror("Error: standard function scanf has failed");
			exit(0);
		}
		last_command = &(*user_input);
	}
	return (last_command);
}

char* gameState_player_vs_AI(char board[BOARD_SIZE][BOARD_SIZE]){
	/*game state - Player vs. Player*/
	char user_input[50] = " ";
	int turn = 0;
	char *last_command;
	int input;
	int j, i;
	struct MovesList *userMove;
	struct MovesList *compBestMove;
	char *userColor;
	char *computerColor;
	char *colorPrint = "White";
	char *compColorPrint = "Black";
	last_command = &(*user_input);
	if (user_color == 0){
		userColor = "white";
		colorPrint = "White";
		computerColor = "black";
		compColorPrint = "Black";
	}
	else{
		userColor = "black";
		colorPrint = "Black";
		computerColor = "white";
		compColorPrint = "White";
	}

	while (strcmp(user_input, "quit") != 0 && strcmp(user_input, "restart") != 0){
		if (current_turn == turn) {
			if (printf("%s player - enter your move:\n", colorPrint) < 0) {
				perror("Error: standard function printf has failed");
				exit(0);
			}
			if (scanf("%s", user_input) != 1) {
				perror("Error: standard function scanf has failed");
				exit(0);
			}
			last_command = &(*user_input);
			if (strcmp(user_input, "move") == 0){/*user enter's a move*/
				userMove = init_movesList(-1, -1);
				if (create_userMove(board, userMove, userColor) == 0){/*move wasn't created*/
					turn = abs(turn - 1);
				}
				else if (isValidMove(board, userMove, userColor, 1) == 0){
					turn = abs(turn - 1);
				}
				else{/*if the move is legal - make the move*/
					changeBoard_SingleMove(board, board, userMove->head->col, userMove->head->row, userMove->tail);
					print_board_(board);
					if (checkForCheckmate(board, userColor) == 1){/*if user won*/
						if (printf("Mate! %s player wins the game\n", colorPrint) < 0) {
							perror("Error: standard function printf has failed");
							exit(0);
						}
						free_movesList(userMove);
						free(userMove);
						break;
					}
					else if (checkForChess(board, userColor) == 1){/*if computer is now under check*/
						if (printf("Check!\n") < 0) {
							perror("Error: standard function printf has failed");
							exit(0);
						}
					}
					free_movesList(userMove);
					free(userMove);
				}
			}
			else if (strcmp(user_input, "get_moves") == 0){/*the user asked for for a piece's possible moves*/
				if (scanf("%s", user_input) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				j = (int)(user_input[1] - 'a');
				i = (int)(user_input[3] - '0') - 1;
				print_userMoves(board, j, i, userColor);
				turn = abs(turn - 1);
			}
			else if (strcmp(user_input, "quit") == 0){/*the user wishes to quit the game*/
				exit(0);
			}
			else if (strcmp(user_input, "save") == 0){/*the user wishes to save the game*/
				save_game(board, 2, min_max_depth, best, user_color, current_turn, NULL);
				turn = abs(turn - 1);
			}
			else{
				while ((input = getchar()) != EOF && input != '\n'){
					if (scanf("%s", user_input) != 1) {
						perror("Error: standard function scanf has failed");
						exit(0);
					}
				}
				print_message(ILLEGAL_COMMAND);
				turn = abs(turn - 1);
			}
			turn = abs(turn - 1);
			}
			else{/*computer's turn*/
				compBestMove = init_movesList(-1, -1);
				compBestMove->head->next = init_move(-1, -1);
				compBestMove->tail = compBestMove->head->next;
				if (best == 1) {
					min_max_depth = 4;
				}
				alphaBetaMax(-10000000, 10000000, min_max_depth, 0, board, computerColor, compBestMove, best);
				if (printf("Computer: move ") < 0) {
					perror("Error: standard function printf has failed");
					exit(0);
				}
				print_Moves(compBestMove);
				changeBoard_SingleMove(board, board, (compBestMove)->head->col, (compBestMove)->head->row, (compBestMove)->head->next);
				print_board_(board);
				free_movesList(compBestMove);
				free(compBestMove);

				if (checkForCheckmate(board, computerColor) == 1){/*if computer won*/
					if (printf("Mate! %s player wins the game\n", compColorPrint) < 0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
					break;
				}
				else if (checkForChess(board, computerColor) == 1){/*if user is now under check*/
					if (printf("Check!\n") <0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
				}
				turn = abs(turn - 1);/*switch to next palyer's turn*/
			}
		}
		return (last_command);/*return last command given by user*/
	}

	char *game_AI_setting(char board[BOARD_SIZE][BOARD_SIZE]){
		/*game setting - commands for set from the user in AI state*/
		char command[100];
		int minMaxDepth = 1;
		char userColor[6] = "white";
		int col, row, i, j, notempty, verify_black, verify_white;
		char place[6];
		int input;
		char color[6];
		char type[10];
		int errorPrinted = 0;
		char *end_command;
		int can_start = 0;
		verify_black = 0;
		verify_white = 0;
		notempty = 0;
		if (printf("Enter game setting:\n") <0) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		if (scanf("%s", command) != 1) {/*command*/
			perror("Error: standard function scanf has failed");
		}
		while (can_start == 0){/*setting loop - go on until game can start + user enters "start"*/
			errorPrinted = 0;
			if (strcmp(command, "difficulty") == 0){
				/*set minmax depth*/
				if (scanf("%s", command) != 1) {
					perror("Error: standard function scanf has failed");
				}
				if (strcmp(command, "depth") == 0){/*choose minimax depth*/
					if (scanf("%d", &minMaxDepth) != 1) {
						perror("Error: standard function scanf has failed");
						exit(0);
					}
					if (minMaxDepth < 1 || minMaxDepth>4){
						print_message(WRONG_MINIMAX_DEPTH);
						min_max_depth = 1;
					}
					else{
						min_max_depth = minMaxDepth;
					}
				}
				else if (strcmp(command, "best") == 0){
					best = 1;
				}
				else{
					print_message(ILLEGAL_COMMAND);
				}
			}
			else if (strcmp(command, "user_color") == 0){/*choose user's color*/
				/*set user's color*/
				if (scanf("%s", userColor) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				if (strcmp(userColor, "white") == 0){
					user_color = 0;
					current_turn = 0;
				}
				else if (strcmp(userColor, "black") == 0){
					user_color = 1;
					current_turn = 1;
				}
				else{
					print_message(WRONG_COLOR);
				}
			}
			else if (strcmp(command, "clear") == 0){
				/*clear the game board*/
				clear(board);
			}
			else if (strcmp(command, "rm") == 0){
				/*remove disc from board*/
				if (scanf("%s", place) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				col = (int)(place[1] - 'a');
				row = (int)(place[3] - '0') - 1;
				rm(board, col, row);
			}
			else if (strcmp(command, "set") == 0){
				/*set a disc on the board*/
				if (scanf("%s", place) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				if (scanf("%s", color) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				if (scanf("%s", type) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				col = (int)(place[1] - 'a');
				row = (int)(place[3] - '0') - 1;
				if (isLegal(col, row) == 1){/*check legal range*/
					set(board, col, row, color, type);
				}
				else {
					print_message(WRONG_POSITION);/*ilegal square*/
				}
			}
			else if (strcmp(command, "print") == 0){
				/*print current board*/
				print_board_(board);
			}
			else if (strcmp(command, "quit") == 0){
				/*quit the game*/
				exit(0);
			}
			else if (strcmp(command, "start") == 0){
				/*start the game - play in game state - check for a legal board*/
				for (i = 0; i < BOARD_SIZE; ++i) {
					for (j = 0; j < BOARD_SIZE; ++j) {
						if (board[j][i] != EMPTY){
							notempty = 1;
						}
						if (verifyColor(board, j, i, "black") == 1){
							verify_black++;
						}
						if (verifyColor(board, j, i, "white") == 1){
							verify_white++;
						}
					}
				}
				if ((notempty == 0) || (verify_black == 0) || (verify_white == 0) || (verify_black > 16) || (verify_white > 16)) {
					print_message(WROND_BOARD_INITIALIZATION);/*illegal to play on this board*/
				}
				else {
					can_start = 1;/*game can start*/
					end_command = gameState_player_vs_AI(board);
					break;/*game over*/
				}
			}
			else{/*illegal command*/
				while ((input = getchar()) != EOF && input != '\n'){
					if (scanf("%s", command) != 1) {
						perror("Error: standard function scanf has failed");
						exit(0);
					}
				}
				print_message(ILLEGAL_COMMAND);
				errorPrinted = 1;
				if (scanf("%s", command) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
			}if (errorPrinted == 0){
				if (scanf("%s", command) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
			}
		}
		return (end_command);/*return last command given in the game state*/

	}

	char* game_setting(char board[BOARD_SIZE][BOARD_SIZE]){
		/*game setting - choose new game mode or load a game*/
		char command[100] = { ' ' };
		char x;
		char *end_command;
		int input;
		int load = 0;
		int gameModeChosen = 0;
		int errorPrinted = 0;
		if (printf("Enter game setting:\n") < 0 ) {
			perror("Error: standard function printf has failed");
			exit(0);
		}
		while (strcmp(command, "quit") != 0 && strcmp(command, "start") != 0 && gameModeChosen == 0){
			errorPrinted = 0;
			if (scanf("%s", command) != 1) {
				perror("Error: standard function scanf has failed");
				exit(0);
			}
			if (strcmp(command, "game_mode") == 0){/*choose game mode*/
				if (scanf(" %c", &x) != 1) {
					perror("Error: standard function scanf has failed");
					exit(0);
				}
				if (load == 0){
					if (x == '1'){/*two players mode*/
						game_mode = 1;
					}
					else if (x == '2'){/*player vs. AI mode*/
						game_mode = 2;
						gameModeChosen = 1;
					}
					else{
						if (errorPrinted == 0) {
							print_message(WRONG_GAME_MODE);
							errorPrinted = 1;
						}
					}
				}
				else{
					if (errorPrinted == 0) {
						print_message(ILLEGAL_COMMAND);
						errorPrinted = 1;
					}
				}
			}
			else if (strcmp(command, "load") == 0){/*load game*/
				if (load_game(board, &game_mode, &min_max_depth, &best, &user_color, &current_turn, NULL) == 0){
					load = 1;
				}
			}
			else{
				if (strcmp(command, "quit") == 0 || strcmp(command, "start") == 0){
					break;
				}
				else{
					while ((input = getchar()) != EOF && input != '\n'){
						if (scanf("%s", command) != 1) {
							perror("Error: standard function scanf has failed");
							exit(0);
						}
					}
					if (errorPrinted == 0) {
						print_message(ILLEGAL_COMMAND);
						errorPrinted = 1;
					}
				}
			}
		}
		if (strcmp(command, "start") == 0 || gameModeChosen == 1){/*start game*/
			if (load == 1){/*start loaded game*/
				if (game_mode == 1){
					print_board_(board);
					end_command = gameState_twoPlayers(board);
				}
				else{
					print_board_(board);
					end_command = gameState_player_vs_AI(board);
				}
			}
			else{/*start a new game*/
				if (game_mode == 1){
					if (printf("Running game in 2 players mode\n") < 0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
					end_command = gameState_twoPlayers(board);
				}
				else if (game_mode == 2){
					if (printf("Running game in player vs. AI mode\n") < 0) {
						perror("Error: standard function printf has failed");
						exit(0);
					}
					end_command = game_AI_setting(board);
				}
				else{
					if (printf("Running game in 2 players mode\n") < 0) {
						perror("Error: standard function has failed");
						exit(0);
					}
					end_command = gameState_twoPlayers(board);
				}
			}
		}
		else if (strcmp(command, "quit") == 0){/*quit game*/
			exit(0);
		}
		return (end_command);
	}

	void game_on(){
		/*main game control - call for setting each game and restart a game*/
		char board[BOARD_SIZE][BOARD_SIZE];
		char *command;
		init_board_(board);
		print_board_(board);
		command = game_setting(board);
		while (strcmp(command, "restart") == 0){
			init_board_(board);
			print_board_(board);
			if (game_mode == 1){
				command = gameState_twoPlayers(board);
			}
			else{
				command = gameState_player_vs_AI(board);
			}
		}
	}


	int main(int argc, char* argv[]) {

		if (argv[1] == NULL) {
			game_on();
		}
		else if (strcmp(argv[1], "gui") == 0) {
			game_on_sdl();
		}
		else if (strcmp(argv[1], "console") == 0) {
			game_on();
		}
		else if (strcmp(argv[1], "") == 0){
			game_on();
		}
		return 0;
	}


