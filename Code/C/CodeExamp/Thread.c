#include <stdio.h>
#include <pthread.h>


void PrintVar(void* arg) {
	int * a = (int *)arg;
	printf("%d\n", *a);
}

/*
int main(int argc, char *argv[]) {
	int a,rc;
	a = 10;
	pthread_t thr;
	pthread_create(&thr, NULL, PrintVar, &a);
	pthread_exit(&rc);
	getchar();
}*/