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
#include <pthread.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <time.h> 


/************* globals **************/

/*matrix variable*/
unsigned char buf[4];
int **mat_1, **mat_2, **mat_res;
int fd_mat1, fd_mat2, fd_res, n;

/*threads variable*/
static int mode, t; /*mode and number of threads*/
int variable = 0; /*global variable for increment*/

/*mutex and shared variable*/
pthread_mutex_t shared_lock; /*mutex for access to shared "data"*/
int done = 0; /* used to signal consumers they should exit gracefuly*/

/*time measurements*/
struct timeval time_begin, time_end;

/************************************/

int bytes_to_number() {
	/*turn 4-byte array to decimal number*/
	int num = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | (buf[0]);
	return num;
}

void number_to_bytes(int num){
	/*turn int into 4-bytes array*/
	buf[0] = num & 0x000000ff;
	buf[1] = (num & 0x0000ff00) >> 8;
	buf[2] = (num & 0x00ff0000) >> 16;
	buf[3] = (num & 0xff000000) >> 24;
}

int fill_matrix(int fd, int** mat, int n){
	/*fill matrix from file*/
	int i, j, offset, value;
	for (j = 0; j < n; j++){
		for (i = 0; i < n; i++){
			offset = (j * 4 * n) + (4 * i) + 4;
			if (lseek(fd, offset, SEEK_SET) < 0) {
				perror("Error - error reading from matrix file!\n");
				close(fd);
				return -1;
			}
			if (read(fd, buf, 4) < 0){/*read value from first mat*/
				perror("Error - error reading from matrix file!\n");
				close(fd);
				return -1;
			}
			value = bytes_to_number();
			mat[i][j] = value;
		}
	}
	return 1;
}

int write_to_file(int fd, int **mat, int n){
	/*write matrix to file*/
	int fd_res, offset;
	int i, j;
	if (lseek(fd, 0, SEEK_SET) < 0) {
		perror("Error - error writing matrix to file!\n");
		close(fd);
		return -1;
	}
	number_to_bytes(n);
	if (write(fd, buf, 4) < 0){
		perror("Error - error writing matrix to file!\n");
		close(fd);
		return -1;
	}
	for (j = 0; j < n; j++){
		for (i = 0; i < n; i++){
			offset = (j * 4 * n) + (4 * i) + 4;
			if (lseek(fd, offset, SEEK_SET) < 0) {
				perror("Error - error writing matrix to file!\n");
				close(fd);
				return -1;
			}
			number_to_bytes(mat[i][j]);
			if (write(fd, buf, 4) < 0){
				perror("Error - error writing matrix to file!\n");
				close(fd);
				return -1;
			}
		}
	}
	return 1;
}

void calc_row(long row){
	/*calculate mul given row*/
	int j, k;
	int var;
	for (j = 0; j < n; j++){/*for each row*/
		var = 0;
		for (k = 0; k < n; k++){
			var += (mat_1[row][k] * mat_2[k][j]);/*add to overall result*/
		}
		mat_res[row][j] = var;/*update result*/
	}
}

void calcMul_mode0(){
	/* calculate mul in mode 0*/
	long i;
	gettimeofday(&time_begin, NULL);
	for (i = 0; i < n; i++){
		calc_row(i);
	}
	gettimeofday(&time_end, NULL);
}

void *thread_mode1(void* args){
	long k;
	for (k = 0; k < n; k++){/*for all the rows*/
		if (k % t == (long)args){
			calc_row(k);
		}
	}
	return NULL;
}

int calcMul_mode1(pthread_t *threads){
	/* calculate mul in mode 1*/
	int i, rc;
	long index;
	gettimeofday(&time_begin, NULL);
	for (i = 0; i < t; i++){
		index = i;
		rc = pthread_create(&threads[i], NULL, thread_mode1, (void*)index);
		if (rc != 0){
			perror("Error - error creating thread!\n");
			return -1;
		}
	}
	for (i = 0; i < t; i++){
		rc = pthread_join(threads[i], NULL);/*wait for threads*/
		if (rc != 0){
			perror("Error - error creating thread!\n");
			return -1;
		}
	}
	gettimeofday(&time_end, NULL);
	return 1;
}

void *thread_mode2(void *args){
	int rc;
	long row;
	while (!done) { /*while process is not done calculating*/
		rc = pthread_mutex_lock(&shared_lock); /* thread gets lock */
		if (rc != 0){
			exit(1);
		}
		if (variable == n){/*if process is over*/
			done = 1;
		}
		row = variable++;
		rc = pthread_mutex_unlock(&shared_lock);/*finshed increasing - release lock*/
		if (rc != 0) {
			exit(1);
		}
		if (row < n){
			calc_row(row);/*calc row*/
		}
	}
	return NULL;
}

int calcMul_mode2(pthread_t *threads){
	/* calculate mul in mode 2*/
	int i;
	gettimeofday(&time_begin, NULL);
	int rc = pthread_mutex_init(&shared_lock, NULL);/*init lock*/
	if (rc != 0){
		perror("Error - error creating lock!\n");
		return -1;
	}
	variable = 0;
	for (i = 0; i < t; i++){/*init threads*/
		rc = pthread_create(&threads[i], NULL, thread_mode2, NULL);
		if (rc != 0){
			perror("Error - error creating thread!\n");
			return -1;
		}
	}
	for (i = 0; i < t; i++){
		rc = pthread_join(threads[i], NULL);/*wait for threads*/
		if (rc != 0){
			perror("Error - error joining thread!\n");
			return -1;
		}
	}
	pthread_mutex_destroy(&shared_lock); /* destroy shared lock */
	gettimeofday(&time_end, NULL);
	return 1;
}

void *thread_mode3(void *args){
	long row;
	row = __sync_fetch_and_add(&variable, 1);
	while (row < n) { /*while process is not done calculating*/
		calc_row(row);
		row = __sync_fetch_and_add(&variable, 1);
	}
	return NULL;
}

int calcMul_mode3(pthread_t *threads){
	/* calculate mul in mode 3*/
	int i, rc;
	long row;
	variable = 0;
	gettimeofday(&time_begin, NULL);
	for (i = 0; i < t; i++){
		rc = pthread_create(&threads[i], NULL, thread_mode3, NULL);
		if (rc != 0){
			perror("Error - error creating thread!\n");
			return -1;
		}
	}
	for (i = 0; i < t; i++){
		rc = pthread_join(threads[i], NULL);/*wait for threads*/
		if (rc != 0){
			perror("Error - error joining thread!\n");
			return -1;
		}
	}
	gettimeofday(&time_end, NULL);
	return 1;
}

void time_measured(){
	/*measure time for mul calculation*/
	long sec_time, micro_time;
	float time_in_sec, time_per_thread;

	sec_time = (time_end.tv_sec - time_begin.tv_sec); /*time diff sec to mili*/
	micro_time = (time_end.tv_usec - time_begin.tv_usec);/*time diff micro to mili*/

	time_in_sec = (float)(sec_time + (micro_time / 1000000.0));/*time in seconds*/
	time_per_thread = (float)(time_in_sec / t);

	printf("multiplication summary: mode: %d, number of threads: %d, total time in seconds: %f, time per thread in seconds: %f\n",mode, t, time_in_sec, time_per_thread);
	return;
}

int init_prog(char** argv){
	/*init the program - read matrixes from file and malloc memory*/
	int i;
	fd_mat1 = open(argv[2], O_RDWR, S_IRWXU);
	if (fd_mat1 < 0){
		perror("Error - error opening first matrix file!\n");
		return -1;
	}
	fd_mat2 = open(argv[3], O_RDWR, S_IRWXU);
	if (fd_mat2 < 0){
		perror("Error - error opening second matrix file!\n");
		close(fd_mat1);
		return -1;
	}
	fd_res = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd_res < 0){
		perror("Error - error creating/opening result matrix file!\n");
		close(fd_mat1);
		close(fd_mat2);
		return -1;
	}
	if (lseek(fd_mat1, 0, SEEK_SET) < 0){
		perror("Error - error reading matrix size!\n");
		close(fd_mat1);
		close(fd_mat2);
		close(fd_res);
		return -1;
	}
	if (read(fd_mat1, buf, 4) < 0){
		perror("Error - error reading matrix size!\n");
		close(fd_mat1);
		close(fd_mat2);
		close(fd_res);
		return -1;
	}
	n = bytes_to_number();/*size of matrix*/

	mat_1 = (int**)malloc(sizeof(int*)* n);
	mat_2 = (int**)malloc(sizeof(int*)* n);
	mat_res = (int**)malloc(sizeof(int*)* n);

	for (i = 0; i < n; ++i){
		mat_1[i] = (int*)malloc(sizeof(int)* n);
		mat_2[i] = (int*)malloc(sizeof(int)* n);
		mat_res[i] = (int*)malloc(sizeof(int)* n);
	}
	if (fill_matrix(fd_mat1, mat_1, n) < 0){
		perror("Error - error reading data from input matrixs file!\n");
		return -1;
	}
	if (fill_matrix(fd_mat2, mat_2, n) < 0){
		perror("Error - error reading data from input matrixs file!\n");
		return -1;
	}

	close(fd_mat1);
	close(fd_mat2);
	return 1;
}

void free_memory(){
	/*free all matrixs*/
	int i;
	for (i = 0; i < n; ++i){
		free(mat_1[i]);
		free(mat_2[i]);
		free(mat_res[i]);
	}
	free(mat_1);
	free(mat_2);
	free(mat_res);
}

int main(int argc, char** argv)
{

	assert(argc >= 5); /*First=number of threads t, second and third = input files containing matrices to multiply, fourth = the output file to write the result into, fifth = provided when t > 0, and specifies the mode */

	/*init step - create all matrixs from file*/
	if (init_prog(argv) < 0){
		return;
	}

	/*threads*/
	int rc, i;
	t = (int)strtol(argv[1], NULL, 10);
	if (argc == 6 && t>0){
		mode = (int)strtol(argv[5], NULL, 10);
	}
	else{
		mode = 0;
	}
	if (mode == 0){/*no threads*/
		calcMul_mode0();
	}
	else{/*threads mode*/
		pthread_t *threads = (pthread_t*)calloc(t, sizeof(pthread_t));/*create t threads*/
		if (threads == NULL){
			free_memory();
			return;
		}
		if (mode == 1){
			if (calcMul_mode1(threads) < 0){
				free(threads);
				free_memory();
				return;
			}
		}
		else if (mode == 2){
			if (calcMul_mode2(threads) < 0){
				free(threads);
				free_memory();
				return;
			}
		}
		else if (mode == 3){
			if (calcMul_mode3(threads) < 0){
				free(threads);
				free_memory();
				return;
			}
		}
		free(threads);
	}
	if (write_to_file(fd_res, mat_res, n) < 0){/*write result to output file*/
		perror("Error - error writing matrix result to file!\n");
		free_memory();
		return;
	}
	time_measured();
	close(fd_res);
	free_memory();
	return;
}


/*************************************************************************************************************************************************************************/



