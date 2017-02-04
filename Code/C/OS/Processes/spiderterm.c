#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char** argv)
{
	char line[1024];
	char words[10][1024];
	char *args[11];

	struct sigaction oldact, newact;
	newact.sa_handler = SIG_IGN;

	int fd;
	int check_fifo = 0;

	struct stat st_fifo;

	assert(argc == 2);/*path to a FIFO file*/

	fd = open(argv[1], O_CREAT | O_RDWR, S_IRWXU);/*open or create FIFO file*/
	if (fd < 0){/*create FIFO file*/
		perror("Error - error opening or creating FIFO file!\n");
		return;
	}
	else{
		/*check file is FIFO file*/
		stat(argv[1], &st_fifo);
		if ((st_fifo.st_mode & S_IFMT) != S_IFIFO){
			if (unlink(argv[1]) < 0){
				perror("Error - error deleting file!\n");
				return;
			}
			if ((fd = mkfifo(argv[1], 0666)) < 0){
				perror("Error - error creating FIFO file!\n");
				return;
			}
		}
	}

	/* read line from stdin*/
	while (fgets(line, 1024, stdin) != NULL) {

		int f = fork(); /*copy process*/

		if (f == 0) { /* inside child process - execute with args*/

			if ((fd = open(argv[1], O_RDONLY)) < 0){/*open FIFO file for reading*/
				perror("Error - error opening FIFO file!\n");
				return;
			}

			if (read(fd, line, 1024) < 0){/*read from FIFO file*/
				perror("Error - error reading from the FIFO file!\n");
				return;
			}

			int num_args = sscanf(line, "%s %s %s %s %s %s %s %s %s %s", words[0], words[1], words[2], words[3], words[4], words[5], words[6], words[7], words[8], words[9]);
			if (num_args == -1){/*empty line*/
				return;
			}

			if (close(fd) < 0){/*close file*/
				perror("Error - error closing the FIFO file!\n");
				return;
			}

			int num = 0;
			while (num < num_args){/*create args*/
				args[num] = (char*)words[num];
				num++;
			}
			args[num] = NULL;

			if (execvp(args[0], args) == -1){/*start chiled process*/
				perror("Error: execv failed!\n");
				return;
			}

		}

		else if (f>0){ /* inside parent process - wait for son to finish, then continue*/

			if ((fd = open(argv[1], O_WRONLY)) < 0){/*open FIFO file for writing*/
				perror("Error - error opening FIFO file!\n");
				return;
			}

			if (write(fd, line, 1024) < 0){/*write to FIFO file*/
				perror("Error - error writing to FIFO file!\n");
				return;
			}

			if (close(fd) < 0){/*close file*/
				perror("Error - error closing the FIFO file!\n");
				return;
			}

			if (sigaction(SIGINT, &newact, &oldact) < 0){/*change to ignore signal*/
				perror("Error: sigaction failed!\n");
				exit(1);
			}

			int status;
			wait(&status);

			if (sigaction(SIGINT, &oldact, NULL) < 0){/*return to default*/
				perror("Error: sigaction failed!\n");
				exit(1);
			}

		}
		else{ /*f<0 - failed*/
			perror("Error: fork failed failed!\n");
			return;
		}
	}
	return 0;
}