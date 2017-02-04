#include <stdio.h>
#include <malloc.h>

char* a1i(int i){
	char* a = (char*)malloc(sizeof(char) * 2); //currection
	a[0] = '0' + i;
	a[1] = '\0';
	printf("%s\n", a);//print 3
	return a;
}

char* a2i(int i){
	// char *a; ---> segmentation fault, must define a[] size
	char a[2]; //currection, but memory doesn't keep
	a[0] = '0'+i;
	a[1] = '\0';
	printf("%s\n", a);//print 3
	return a;
}


/*int* main(){
	char*  a = a1i(3);
	printf("%s\n", a);//print 3
	free(a);
	char*  b = a2i(3);
	printf("%s\n", b);//print nothing
	getchar();
	return 1;
}*/