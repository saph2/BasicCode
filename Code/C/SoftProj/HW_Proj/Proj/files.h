#define FILES_H_

#include "chessprog.h"
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <stdlib.h>
#include <ctype.h>

FILE* createXML(char* file_name);
char* load_file_to_memory(char* xml_file);
int load_field(char* file, char *field);
char* load_color_turn_difficulty(char* file, char *field);
int load_line(char board[BOARD_SIZE][BOARD_SIZE], char* file, char *field, int row);
int load_board(char board[BOARD_SIZE][BOARD_SIZE], char* file);
int write_board(char board[BOARD_SIZE][BOARD_SIZE], FILE *xml_file);
int write_game_settings(FILE *xml, int type, int difficulty, int best, char *user_color, char *next_turn);