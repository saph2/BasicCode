#include <sys/types.h>
#include <sys/wait.h> // for wait macros etc
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char** argv)
{
	int f = fork();
	assert(f >= 0);
	
	if (f > 0) {
		printf("parent pid=%d, child pid=%d\n", getpid(), f);
		while (1);
	}
}
