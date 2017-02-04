#include <stdio.h>


union Data{ //keeps them all in one memory area
	int i;
	char c;
	char* s;
}typedef Data;



/*int main(){
	Data data;
	
	data.i = 5;
	printf("data.i : %d\n", data.i);
	data.c = 'c';
	printf("data.c : %c\n", data.c);
	data.s = "string";
	printf("data.s : %s\n", data.s);
	
	printf("%d %c %s\n", data.i, data.c, data.s);//str currupts the previes ones, only "string" is now OK
	
	getchar();
	return 1;
}*/