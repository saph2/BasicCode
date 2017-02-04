#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define NUM (0xFFFFFFFF)

pthread_t tid[2];

void* doSomeThing(void *arg)
{
    unsigned long i = 0;
    pthread_t id = pthread_self();

    if(pthread_equal(id,tid[0]))
        printf("\n First thread processing\n");
    else
        printf("\n Second thread processing\n");

    for(i=0; i < NUM; i++);

    return 0;//pthread_exit(NULL);
}

int main(void)
{
    int i = 0;
    int err;

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\nCan't create thread :[%s]", strerror(err));
        else
            printf("\nThread created successfully\n");

        i++;
    }

    pthread_exit(NULL);//return 0;
}
