#include <stdio.h>
#include <string.h>

char* rv(char* str){
	int len = strlen(str);
	int m = len / 2;
	char temp;
	int i;
	for (i = 0; i < m; i++){
		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}
	return str;
}

char* ml(char* str, int k){
	int len = strlen(str);
	char* newStr = (char*)calloc((len+1)*k, sizeof(char));
	int i, j;
	for (i = 0; i < k; i++){
		for (j = 0; j < len; j++){
			newStr[len*i+j] = str[j];
		}
	}
	return newStr;
}

int main(){

	char *str = "hello";
	char *newStr;
	char tempStr[1024];
	strncpy(tempStr, str, 1024);
	printf("copy: %s\n", tempStr);//"hello"
	newStr = rv(tempStr);
	printf("rv: %s\n", newStr);
	newStr = ml(str,3);
	printf("ml: %s\n", newStr);
	char* result = strstr("hello to you", "hello"); //"hello to you" on match
	printf("Strstr: %s\n", result);
	result = strstr("hello", "hi");
	printf("Strstr: %s\n", result);//null if no match found
	int numres = strcmp("hello", "hello");
	printf("cmp: %d\n", numres);//0 on seccesss
	getchar();
	return 1;
}