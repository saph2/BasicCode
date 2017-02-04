#include <stdio.h>
#include <string.h>
#include <malloc.h>

int fun1(){
	return 1;
}

struct Node{
	int x;
	char c;
}typedef Node;


void changeNode(Node* n){
	n->x = 1;
	n->c = '2';
}

/*
int main(){
	int x = 5;
	int* p = &x; // *p=x //
	int** q = &p; // *q=p **q=*p=x //

	int(*fun1);

	printf("hi "), printf("hello"); //prints "hi hello" //

	char words[20];
	char two[2] = "hi";
	strncpy(words, two, 20);//space left //
	printf("%s\n", words);

	char *s1 = "hello", *s2 = "hi";
	//strcat(s1, s2);//s1 points to a string constant and cannot be altered //

	char s3[50];
	char s4[2];
	strcpy(s3, "hello");
	strcpy(s4, "hi");
	strcat(s3, s4); //ok - hellohi //
	printf("%s\n", s3);

	char arr[3][3] = { { '1', '2' }, { '3', '4' } };
	printf("%c\n", **arr);//prints 1 //
	printf("%c\n", **arr + 1);//prints 2 //
	printf("%s\n", *arr);//prints 12 - size 3 for string! //
	printf("%s\n", *arr + 1);//prints 2 //
	printf("%s\n", *arr + 2);//prints nothing //
	printf("%s\n", *arr + 3);//prints 34 //

	int a[3] = { 1, 2, 3 };
	int b[3] = { 2, 3, 4 };
	int* v = a;
	int* u = b;
	*v = NULL;//v points to the array itself to a[0] - will change array to {null, 2,3} //

	Node *node = (Node*)malloc(sizeof(Node));
	changeNode(node);
	printf("%d\n", node->x);//prints 1 //

	Node n = { 5, 'c' }; //init at creatio //
	changeNode(&n);
	printf("%d\n", n.x);//prints 1 //

	int len = 3;
	char* str = (char*)calloc(len,sizeof(char));
	str[0] = 'h', str[1] = 'i';
	char strArr[1024];
	strcpy(strArr, str);
	len = strlen(str);
	printf("len is: %d\n", len);//prints len is: 2 //
	len = strlen(strArr);
	printf("len is: %d\n", len);//prints len is: 2 //

	getchar();
	return 1;
} */