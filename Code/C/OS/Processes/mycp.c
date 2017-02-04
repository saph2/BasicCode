#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


int fd[2];
char *arr_s;
char *arr_d;

void map_files(char *s_path, char *d_path){
	size_t source_size;
	int check;
	fd[0]=open(s_path,O_RDWR|O_CREAT);/*open our source file*/
	if (fd[0]<0){
		perror ("Error opening source file for reading\n");
		exit (EXIT_FAILURE);
	}
	fd[1]=open(d_path,O_RDWR);/*check if destination file exists*/
	if (fd[1]>=0){
		close (fd[0]);
		close (fd[1]);
		perror ("Error destination file already exists\n");
		exit (EXIT_FAILURE);
	}
	
	fd[1]=open(d_path,O_CREAT|O_TRUNC|O_RDWR,S_IRWXU);/*create destination file*/
	if(fd[1]<0){
		close (fd[0]);
		perror ("Error opening destination file for writing\n");
		exit (EXIT_FAILURE);
	}
	source_size=lseek(fd[0],-1,SEEK_END);/*go to end of file to get size*/
	if (source_size == -1){
		close (fd[0]);
		close (fd[1]);
		unlink(d_path);
		printf ("Error calling lseek() to the end of the file: %s\n",s_path);
		exit (EXIT_FAILURE);	
	}
	int tr=truncate(d_path,source_size);/*truncate file*/	
	check=lseek(fd[1],source_size-1,SEEK_SET);/*go to end of file*/
	if (check == -1){
		close (fd[0]);
		close (fd[1]);
		printf ("Error calling lseek() to the end of the file: %s\n",d_path);
		exit (EXIT_FAILURE);	
	}	
	int write_bytes=write(fd[1],"",1);/*write in the end of destination file*/
	if (write_bytes!=1){
		close (fd[0]);
		close (fd[1]);
		unlink(d_path);
		printf ("Error writing last byte to the file: %s\n",d_path);
		exit (EXIT_FAILURE);	
	}
	check=lseek(fd[0],0,SEEK_SET);/*go to start of file*/
	if (check == -1){
		close (fd[0]);
		close (fd[1]);
		printf ("Error calling lseek() to start of the file: %s\n",s_path);
		exit (EXIT_FAILURE);	
	}
	check=lseek(fd[1],0,SEEK_SET);/*go to start of file*/
	if (check == -1){
		close (fd[0]);
		close (fd[1]);
		printf ("Error calling lseek() to start of the file: %s\n",d_path);
		exit (EXIT_FAILURE);	
	}	
	arr_s= (char*) mmap(NULL,source_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd[0],0);/*mmapp source file*/
	if (arr_s==MAP_FAILED){
		close (fd[0]);
		close (fd[1]);
		printf("Error mmapping the file: %s\n",s_path);
		exit(-1);			
	}
	arr_d= (char*) mmap(NULL,source_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd[1],0);/*mmapp destination file*/
	if (arr_d==MAP_FAILED){
		close (fd[0]);
		close (fd[1]);
		unlink(d_path);
		printf("Error mmapping the file: %s\n",d_path);
		exit(-1);			
	}
	int i;
	for (i=0; i<=source_size; ++i){/*copy data from source arr to destination arr*/
		arr_d[i]=arr_s[i];
	}
	int end;
	end=munmap(arr_s,source_size);/*unmmapp source file*/
	if(end==-1){
		close (fd[0]);
		close (fd[1]);
		unlink(d_path);
		printf("Error un-mmapping the file: %s\n",s_path);
		exit(-1);
	}
	end=munmap(arr_d,source_size);/*unmmapp destination file*/
	if(end==-1){
		close (fd[0]);
		close (fd[1]);
		unlink(d_path);
		printf("Error un-mmapping the file: %s\n",d_path);
		exit(-1);
	}
}


int main(int argc, char **argv){
	assert(argc>=1);
	map_files(argv[1],argv[2]);
	close (fd[0]);
	close (fd[1]);
	return 0;
}
