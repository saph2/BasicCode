#include <stdio.h>


int main(){

	int i = 0;
	int j = 0;
	
	do{
		i++;
	} while (i < 10);

	printf("i: %d\n", i); //print 10 //

	if (j == 0){
		goto loop;
	}


loop: while (j < 10){	
	if (j == 5){
		goto done;
	}
	goto add;
}

  add: { j++; goto loop; }

  done:;

	printf("j: %d\n", j); //print 5 //

	getchar();
	return 1;
}