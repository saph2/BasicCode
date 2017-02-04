#include <stdio.h>
#include <math.h>

int atoiNew(char* str){
	int len = strlen(str);
	int i;
	int num = 0;
	int j = 0;
	int dig;
	for (i = len-1; i >= 0; i--){
		dig = (str[i] - 48);
		num += pow(10, j) * dig;
		j++;
	}
	return num;
}

/*int main(){
	char* str="10";
	printf("%d\n", atoiNew(str));
	getchar();
	return 1;
}*/