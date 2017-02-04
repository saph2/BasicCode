#include <stdio.h>

enum week { sunday, monday, tuesday, wednesday, thursday, friday, saturday } typedef week;

enum time { 
	night = 10,
	day = 20
} typedef time;


enum designFlags {
	ITALICS = 1, //0001
	BOLD = 2, //0010
	UNDERLINE = 4 //0100
} typedef button;


/*int main(){
	
	week today;	
	today = wednesday;
	printf("Day: %d\n", today + 1); //4

	time tod;
	tod = night;
	printf("Time: %d\n", tod); //10

	button b;
	b = ITALICS | BOLD; //0011
	printf("Style: %d\n", b); //3


	getchar();
	return 1;
}*/