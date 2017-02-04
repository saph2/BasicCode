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
#define SIZE_4KB (4*KB)
static char buff_input[SIZE_4KB];
static char buff_key[SIZE_4KB];
static char buff_output[SIZE_4KB];

int decrypted_encrypted(int f_input, int f_key, int f_output){
	/* decrypt or encrypt the file by the key and output the result */
	int i, k, p, offset_input, offset_key;
	size_t nbytes = SIZE_4KB;
	ssize_t input_bytes, key_bytes, out_bytes;
	i = 0;
	offset_input = 0;
	offset_key = 0;
	assert((input_bytes = read(f_input, buff_input, nbytes)) != -1);
	while (input_bytes!=0){
		/*go over input file until last byte - in 4KB jumps*/
		assert((lseek(f_key, offset_key, SEEK_SET)) != -1);
		assert((key_bytes = read(f_key, buff_key, nbytes)) != -1);
		p = 0;
		k = 0;
		while (p <= (int) input_bytes){/*xor each byte*/
			if (k >= (int) key_bytes){ /*end of key file - restart key*/
				offset_key = 0;/*restart key file*/
				assert((lseek(f_key, offset_key, SEEK_SET)) != -1);
				assert((key_bytes = read(f_key, buff_key, nbytes)) != -1);
				k = 0;
			}
			buff_output[p] = (buff_input[p] ^ buff_key[k]);
			if (p < (int)input_bytes){
				k++;
			}
			p++;	
		}
		offset_key = offset_key + k;/*update key offset*/
		
		assert((lseek(f_output, offset_input, SEEK_SET)) != -1);/*write next 4KB from file*/
		assert((out_bytes = (write(f_output, buff_output, input_bytes))) != -1);/*write to output file*/
		
		i++;/*update input offset*/
		offset_input = i*SIZE_4KB;
		
		assert((lseek(f_input, offset_input, SEEK_SET)) != -1);/*read next 4KB from file*/
		assert((input_bytes = read(f_input, buff_input, nbytes)) != -1);
		
		
	}
	close(f_input);
	close(f_key);
	close(f_output);
	return 1;
}

int main(int argc, char* argv[]){
	int f_input, f_key, f_output;
	assert(argc == 4);/*file - key - result file*/
	assert((f_input = open(argv[1], O_RDWR)) > 0);
	assert((f_key = open(argv[2], O_RDONLY)) > 0);
	assert((f_output = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) > 0);
	decrypted_encrypted(f_input, f_key, f_output);
	return 1;
}
