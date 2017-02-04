#include <stdio.h>
#include <malloc.h>

struct Node{
	int data;
	struct Node* next;
} typedef Node;

struct List{
	struct Node* head;
}typedef List;

List* reveseList(List* lst){
	Node* current = lst->head;
	Node* prev= NULL;
	Node* next= NULL;

	while (current != NULL){ //1->2->3-> NULL
		next = current->next; //next = 2 ; next = 3; next = NULL
		current->next = prev; //1-> NULL ; 2 -> 1; 3 - > 2
		prev = current; //prev = 1; prev = 2; prev = 3;
		current = next; //current = 2; current = 3; current = NULL
	}

	lst->head = prev; //head -> 3
	return lst;
}

List* creatList(){
	List* lst = (List*)malloc(sizeof(List));
	int i;
	lst->head = (Node*) malloc(sizeof(Node));
	Node* current = lst->head;
	Node* prev;
	for (i = 1; i < 5; i++){
		current->data = i;
		current->next = (Node*)malloc(sizeof(Node));
		prev = current;
		current = current->next;		
	}
	current = NULL;
	prev->next = NULL;
	return lst;
}

void freeList(List* lst){
	Node* current = lst->head;
	while (current != NULL){
		Node* temp = current;
		current = current->next;
		free(temp);
	}
	free(lst);
}

void printList(List* lst){
	Node* current = lst->head;
	while (current != NULL){
		printf("%d", current->data);
		current = current->next;
	}
}

//void main(){
//	List* lst = creatList();
//	printList(lst);
//	lst = reveseList(lst);
//	printf("\n");
//	printList(lst);
//	freeList(lst);
//	getchar();
//}



