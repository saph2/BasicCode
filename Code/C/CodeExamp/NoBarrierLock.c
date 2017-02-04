#include <pthread.h>
#include <stdio.h>
#include <assert.h>

pthread_mutex_t lock;
pthread_cond_t cond_cahnged;

int cond = 1;
int n = 5;
int counter = 0;
int rc;
int done = 0;
int visit[5] = {0,0,0,0,0};

int check(){
	int i;
	for (i = 0; i < 5; i++){
		if (visit[i] == 0){
			return 0;
		}
	}
	return 1;
}

void run_thread(int args){
	int index = args;
	while (!done){
		rc = pthread_mutex_lock(&lock); //lock
		assert(!rc);
		visit[index] = 1;
		if (check() == 1){
			done = 1;
		}
		rc = pthread_mutex_unlock(&lock); //unlock
		assert(!rc);
	}	
}


/*int main(){
	pthread_t *t = (pthread_t*) malloc(sizeof(pthread_t)*n);
	rc = pthread_mutex_init(&lock, NULL); 
	assert(rc == 0);
	rc = pthread_cond_init(&cond_cahnged, NULL);
	assert(rc == 0);
	int i;
	for (i = 0; i < n; i++){
		pthread_create(&(t[i]), NULL, run_thread, (void*) i);
	}
	for (i = 0; i < n; i++){
		pthread_join(t[i], NULL);
	}
	for (i = 0; i < 5; i++){
		printf("Thread[%d]: %d \n", i, visit[i]);
	}
	printf("Finish!\n");
	getchar();
	return 1;
}*/