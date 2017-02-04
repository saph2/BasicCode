#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>

#define SIZE 14
#define MAX 1024

char writeW[SIZE] = "hello to you ";
char readW[MAX];
int fd;

void writeToFile(char* str){
	FILE* file = fopen(str, "r+");
	if (file == NULL){
		file = fopen(str, "w+");
	}
	assert(file != NULL);
	fd = fseek(file, 0, SEEK_END);
	assert(fd == 0);
	fd = fputs(writeW, file);
	assert(fd == 0);
	fd = fclose(file);
}

void readFromFile(char* str){
	FILE* file = fopen(str, "r");
	fd = fgets(readW, MAX, file);
	assert(fd > 0);
	printf("%s\n", readW);
	fd = fclose(file);
}


int main(){
	writeToFile("test.txt");
	writeToFile("test.txt");
	readFromFile("test.txt");
	printf("Finish!\n");
	getchar();
	return 1;
}