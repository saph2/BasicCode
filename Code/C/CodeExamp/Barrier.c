#include <pthread.h>
#include <stdio.h>

pthread_barrier_t barrier;

void run_fun(int* args){
	printf("Thread %d now\n", *args);
	pthread_barrier_wait(&barrier);
}



/*int main(){
	pthread_t t1,t2;
	int a = 1, b = 2;
	pthread_barrier_init(&barrier, NULL, 3);
	pthread_create(&t1, NULL, run_fun, &a);
	pthread_create(&t2, NULL, run_fun, &b);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("Finish!\n");
	getchar();
	return 1;
}*/