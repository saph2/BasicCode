#include "files.h"

FILE* createXML(char* file_name) {
	FILE *xml_file;
	xml_file = fopen(file_name, "w+");
	if (xml_file == NULL) {
		freopen(file_name, "w+", xml_file);
	}
	if (fprintf(xml_file, "%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") < 0){
		perror("Error: standard function fprintf has failed");
		exit(0);
	}
	return xml_file;
}

int write_game_settings(FILE *xml, int type, int difficulty, int best, char *user_color, char *next_turn){
	char *diff;
	char number[5] = { 0 };
	if (fprintf(xml, "%s\n", "<game>") < 0) {
		perror("Error: standard function fprintf has failed");
		exit(0);
	}
	if (best == 1){
		diff = "Best";
	}
	else{
		number[0] = (char)(difficulty + '0');
		diff = number;
	}
	if (type == 2) {
		if (strcmp(user_color, "black") == 0){
			if (fprintf(xml, "\t%s%d%s\n\t%s%s%s\n\t%s%s%s\n\t%s%s%s\n", "<type>", type, "</type>", "<difficulty>", diff, "</difficulty>", "<user_color>", "Black", "</user_color>", "<next_turn>", "Black", "</next_turn>") < 0) {
				perror("Error: standard function fprintf has failed");
				exit(0);
			}
		}
		else{
			if (fprintf(xml, "\t%s%d%s\n\t%s%s%s\n\t%s%s%s\n\t%s%s%s\n", "<type>", type, "</type>", "<difficulty>", diff, "</difficulty>", "<user_color>", "White", "</user_color>", "<next_turn>", "White", "</next_turn>") < 0){
				perror("Error: standard function fprintf has failed");
				exit(0);
			}
		}
	}
	else{
		if (strcmp(next_turn, "black") == 0){
			if (fprintf(xml, "\t%s%d%s\n\t%s%s%s\n", "<type>", type, "</type>", "<next_turn>", "Black", "</next_turn>") < 0){
				perror("Error: standard function fprintf has failed");
				exit(0);
			}
		}
		else{
			if (fprintf(xml, "\t%s%d%s\n\t%s%s%s\n", "<type>", type, "</type>", "<next_turn>", "White", "</next_turn>") < 0){
				perror("Error: standard function fprintf has failed");
				exit(0);
			}
		}
	}
	return 0;
}

int write_board(char board[BOARD_SIZE][BOARD_SIZE], FILE *xml_file) {
	int i, j;
	if (fprintf(xml_file, "\t%s\n", "<board>") < 0){
		perror("Error: standard function fprintf has failed");
		exit(0);
	}
	for (j = BOARD_SIZE - 1; j >= 0; j--) {
		if (fprintf(xml_file, "\t\t%s%d%s", "<row_", j+1, ">") < 0){
			perror("Error: standard function fprintf has failed");
			exit(0);
		}
		for (i = 0; i < BOARD_SIZE; i++)  {
			if (board[i][j] == EMPTY) {
				if (fprintf(xml_file, "%c", '_') < 0) {
					perror("Error: standard function fprintf has failed");
					exit(0);
				}
			}
			else{
				if (fprintf(xml_file, "%c", board[i][j]) < 0){
					perror("Error: standard function fprintf has failed");
					exit(0);
				}
			}
		}
		if (fprintf(xml_file, "%s%d%s", "</row_", j+1, ">") < 0){
			perror("Error: standard function fprintf has failed");
			exit(0);
		}
		if (fprintf(xml_file, "%c", '\n') < 0) {
			perror("Error: standard function fprintf has failed");
			exit(0);
		}
	}
	if (fprintf(xml_file, "\t%s\n", "</board>") < 0) {
		perror("Error: standard function fprintf has failed");
		exit(0);
	}
	if (fprintf(xml_file, "%s\n", "</game>") < 0){
		perror("Error: standard function fprintf has failed");
		exit(0);
	}
	return 0;

}

int load_field(char* file, char *field) { /* returns difficulty or type. field is <type> or <difficulty>*/
	char *returned;
	int num;
	returned = strstr(file, field) + strlen(field);
	num = returned[0] - '0';
	return num;
}

char* load_color_turn_difficulty(char* file, char *field) { /* returns color or next_turn. field is <color> or <next_turn>*/
	char *result;
	char *color_or_diff;
	char number[2] = { 0 };
	result = strstr(file, field) + strlen(field);
	if (strcmp(field, "<difficulty>") == 0){
		if (result[0] == 'B') {
			color_or_diff = "best";
		}
		else{
			number[0] = result[0];
			color_or_diff = number;
		}
	}
	else{
		if (result[0] == 'W') {
			color_or_diff = "white";
		}
		else {
			color_or_diff = "black";
		}
	}
	return color_or_diff;
}

int load_line(char board[BOARD_SIZE][BOARD_SIZE], char* file, char *field, int row) {
	char *line;
	int j;
	line = strstr(file, field) + strlen(field);
	for (j = 0; j < BOARD_SIZE; ++j){
		if (line[j] == '_'){
			board[j][row] = EMPTY;
		}
		else{
			board[j][row] = line[j];
		}
	}
	return 0;
}

int load_board(char board[BOARD_SIZE][BOARD_SIZE], char* file){
	load_line(board, file, "<row_8>", 7);
	load_line(board, file, "<row_7>", 6);
	load_line(board, file, "<row_6>", 5);
	load_line(board, file, "<row_5>", 4);
	load_line(board, file, "<row_4>", 3);
	load_line(board, file, "<row_3>", 2);
	load_line(board, file, "<row_2>", 1);
	load_line(board, file, "<row_1>", 0);
	return 0;
}
