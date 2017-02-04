#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <time.h> 
#include <assert.h>
#include <errno.h> 
#include <dirent.h>
#include <unistd.h>

#define KB 1024
#define SIZE_1MB (KB*KB)
#define SIZE_128MB (128*KB*KB)
static char buff[SIZE_1MB] __attribute__((__aligned__(4096)));

void print_throughput_for_rewrite(struct timeval time_begin, struct timeval time_end, int size){
	/*pritns the time diff in miliseconds and the throughput of the current stage*/
	long sec_to_mili, micro_to_mili,time_in_mil;
	float time_in_sec, throughput;

	sec_to_mili = (time_end.tv_sec - time_begin.tv_sec); /*time diff sec to mili*/
	micro_to_mili = (time_end.tv_usec - time_begin.tv_usec);/*time diff micro to mili*/

	time_in_sec = (float)(sec_to_mili + (micro_to_mili/1000000.0));/*time in seconds*/

	sec_to_mili = (time_end.tv_sec - time_begin.tv_sec) * 1000; /*time diff sec to mili*/
	micro_to_mili = (time_end.tv_usec - time_begin.tv_usec) / 1000.0;/*time diff micro to mili*/
	
	time_in_mil = (sec_to_mili+micro_to_mili+0.5);/*time in miliseconds*/
	
	throughput = (float)((size*(SIZE_128MB / size)) / (time_in_sec*(SIZE_1MB)));/*throughput*/
	printf("stage summary: time in milisec: %ld, time in seconds: %f, throughput: %f\n", time_in_mil,time_in_sec,throughput);
}

int fill_existing_file(int fd, int size, int aligned){
	/*fill file up to sizeMB*/
	int i, offset,num_times;
	struct timeval time_begin, time_end;
	size_t nbytes = SIZE_1MB;
	num_times = SIZE_128MB / size;
	gettimeofday(&time_begin, NULL);
	for (i = 0; i < num_times; i++)/*fill file with 128 MB*/
	{
		if (aligned == 1){
			offset = (int)((rand() & (num_times - 1)) * size);/*random offset - aligned*/
		}
		else{
			offset = (int)(rand() % (SIZE_128MB - size));/*random offset - unaligned*/
		}
		assert((lseek(fd, offset, SEEK_SET)) != -1);
		assert((write(fd, buff, size)) != -1);/*fill file with sizeMB*/
	}
	gettimeofday(&time_end, NULL);
	close(fd);
	print_throughput_for_rewrite(time_begin, time_end, size);
	return 1;
}

int fill_new_file(int fd, int size){
	/*fill new empty file with sizeMB*/
	int i;
	for (i = 0; i < SIZE_128MB/size; i++){
		assert((write(fd, buff, size)) != -1);
	}
	close(fd);
	return 1;
}

int open_file_fill_for_size(char *path, int size, int aligned, int direct){
	/*open file from path or create file on path with 128MB random data*/
	int fd, i;
	struct stat fileSize;
	if (direct == 1){
		fd = open(path, O_CREAT | O_RDWR | O_DIRECT, S_IRWXU);/*open file*/
	}
	else{
		fd = open(path, O_CREAT | O_RDWR, S_IRWXU);/*open file- no DIRECT*/
	}
	assert(fd != -1);
	fstat(fd, &fileSize);
	if ((fileSize.st_size) == 0){/*new empty file*/
		fill_new_file(fd, size);
	}
	else{/*rewrite file*/
		close(fd);
		if (direct == 1){
			fd = open(path, O_TRUNC | O_RDWR | O_DIRECT, S_IRWXU);/*open file*/
		}
		else{
			fd = open(path, O_TRUNC | O_RDWR, S_IRWXU);/*open file- no DIRECT*/
		}
		assert(fd != -1);
		
		fill_existing_file(fd, size, aligned);/*fill file up to sizeMB*/
	}
	return 1;
}

int main(int argc, char* argv[]){
	int i, j, size, aligned, direct;
	assert(argc == 2);
	for (i = 0; i < SIZE_1MB; i++){/*fill buffer*/
		buff[i] = (char)(random() % 255);
	}
	for (i = 0; i < 3; i++){
		size = SIZE_1MB;
		if (i == 0){/*first part*/
      printf("\n ==================== Aligned Decreasing With O_DIRECT flag ======================\n");
			aligned = 1;
			direct = 1;
		}
		else if (i == 1){/*second part for unalight*/
      printf("\n ==================== Unaligned Decreasing With O_DIRECT flag ====================\n");
			aligned = 0;
			direct = 1;
		}
		else{/*thired part for aligth without direct*/
      printf("\n ==================== Aligned Decreasing Without O_DIRECT flag ====================\n");
			aligned = 1;
			direct = 0;
		}
		for (j = 0; j < 5; j++){/*the loop for all sizes*/
			printf("Current stage: file: %s, wriet size in KB: %d, aligned: %d, direct: %d\n", argv[1], size/KB, aligned, direct);
			if ((open_file_fill_for_size(argv[1], size, aligned, direct)) != 1){
				printf("failed to run program for: file path: %s, write size in KB: %d, aligned: %d, direct: %d\n", argv[1], size/KB, aligned, direct);
				return 0;
			}
			size = size >> 2;/*decrease size of MB to KB*/
		}
	}
	return 1;
}
