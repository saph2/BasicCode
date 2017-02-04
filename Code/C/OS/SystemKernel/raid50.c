#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define DEVICE_SIZE (1024*1024*256)

#define SECTOR_SIZE 8
#define SECTOR_PER_BLOCK 2
#define BLOCK_SIZE (SECTOR_SIZE*SECTOR_PER_BLOCK)

#define BUFFER_SIZE (BLOCK_SIZE*256)

char buf[BUFFER_SIZE];
char temp_buf[BUFFER_SIZE];
char keep_buf[BUFFER_SIZE];
int num_raids;/*number of raids*/
int num_disks;/*disks per raid*/
int *dev_fd;
char **disks_paths;
int num_dev; /*total number of devices*/
int num_logical_dev; /*total number of logical devices*/
int power_2[8]={1,2,4,8,16,32,64,128};

int byte_value(int start){
	/*turn byte to decimal number*/
	int i;
	int value=0;
	for (i=0; i<8; i++){
		value+=(int)(buf[start+(7-i)])*power_2[i];
	}
	return value;	 
}

void do_raid50_setbuf(int byte_value){
	int i, j, div_val, mod_val;
	div_val = byte_value;
	char number[8];
	for (i = 0; i < 8; i++){
		mod_val = div_val % 2;
		div_val = div_val / 2;
		number[7 - i] = (char) mod_val;/*fill byte from lsb to msb*/
	}
	for (j = 0; j < BUFFER_SIZE; j++){
		buf[j] = (char) number[j % 8];
	}
	printf("Buffer was set to value: %d\n",byte_value);
}

int xor_disks_in_raid(int dev_num, int xor_disk, int offset, int size, int w_op){
	/*xor all disks in raid in some offset to one buffer*/
	printf("Now working on raid: %d\n", xor_disk/num_disks);
	if (w_op == 0){	/*on write operation work only with buf*/
		printf("Now accessing disk: %d\n",dev_num);
		if (offset != lseek(dev_fd[dev_num], offset, SEEK_SET)) return dev_num;
		if (size != read(dev_fd[dev_num], buf, size)) return dev_num;
	}
	int num, k, j;
	for (k = 0; k < num_disks; k++){
		num = ((dev_num / num_disks)*num_disks) + k;/*first disk in raid*/
		if (num != xor_disk && num != dev_num){
			printf("Now accessing disk: %d\n",num);
			/*printf("offset: %d, size: %d\n", offset,size);*/
			if (offset!=lseek(dev_fd[num], offset, SEEK_SET)) return num;
			if (size !=read(dev_fd[num], temp_buf, size) ) return num;
			for (j = 0; j < size; j++){/*xor the bits*/
				buf[j] = buf[j] ^ temp_buf[j];
			}
		}
	}
	return -1;
}

int repair_with_xor(int dev_num, int xor_disk, int offset, int size){
	/*repair failed disk*/
	if (xor_disks_in_raid(dev_num, xor_disk, offset, size,0) != -1){
			return dev_num;
		}
	else{
		printf("Now accessing disk: %d\n",xor_disk);
		if (offset != lseek(dev_fd[xor_disk], offset, SEEK_SET)) return dev_num;
		if (size != write(dev_fd[xor_disk], buf, size)) return dev_num;
	}
	return -1;
}

int write_and_update_parity(int dev_num, int parity_disk, int offset, int size){
	printf("Write operation on disk: %d\n", dev_num);
	int failed_disk;
	int failed_dev=0;	
	if ((failed_disk = xor_disks_in_raid(dev_num, parity_disk, offset, size, 1)) != -1){/*update parity*/
		/*if update failed - some other disk in the raid, not dev_num, has failed*/
		if (read_failed(failed_disk, offset, size) == -1){/*failed disk restoration on buf*/
			printf("Now accessing disk: %d\n",dev_num);
			if (offset==lseek(dev_fd[dev_num], offset, SEEK_SET)){
				if (size == write(dev_fd[dev_num], keep_buf, size))/*write new data to disk*/{
					if (xor_disks_in_raid(failed_disk, parity_disk, offset, size, 1) != -1){/*update parity as if failed disk is the new data*/
						return dev_num;
					}
				}
				else return dev_num;
			}
			else return dev_num;
		}
		else return dev_num;
	}
	else{
		printf("Now accessing disk: %d\n",dev_num);
		if ((offset != lseek(dev_fd[dev_num], offset, SEEK_SET)) || (size != write(dev_fd[dev_num], keep_buf, size))){
			failed_dev=1;/*write failed - but was ok working only with buffer*/
		}
		printf("Now accessing disk: %d\n",parity_disk);
		if (offset != lseek(dev_fd[parity_disk], offset, SEEK_SET)) return parity_disk;/*write data to parity disk*/
		if (size != write(dev_fd[parity_disk], buf, size)) return parity_disk;
	}
	if (failed_dev==1){
		printf("Writing to disk: %d (restored) ended successfully!\n", dev_num);	
	}
	else{
		printf("Writing to disk: %d ended successfully!\n", dev_num);
	}
	return -1;
}

int read_failed(int dev_num,int offset, int size){
	/*read failed -read and xor the bits from the other disks to the read_buffer*/
	printf("Read operationg on failed disk %d\n", dev_num);
	int i, j, first_dev;
	int raid_num=dev_num/num_disks;
	for (j = num_disks * raid_num; j < num_disks * raid_num + num_disks; j++){/*first disk for xor*/
		if (j != dev_num){
			first_dev = j;
			break;
		}
	}
	if (xor_disks_in_raid(first_dev, dev_num, offset, size, 0) != -1){/*restoration of failed disk has failed*/
		printf("Error! could not perform read operation on failed disk: %d\n", dev_num);		
		return dev_num;
	}
	printf("Reading from disk: %d (restored) ended successfully!\n", dev_num);
	return -1;
}

int do_raid50_repair(int dev_num){
	/*repair disk*/
	int raid_num = dev_num / num_disks;

	printf("Repair operation on disk %d\n", dev_num);
	
	printf("Now accessing disk: %d\n",dev_num);
	if (close(dev_fd[dev_num]) < 0) return dev_num;
	dev_fd[dev_num] = -1;
	if ((dev_fd[dev_num] = open(disks_paths[dev_num], O_RDWR)) < 0) return dev_num;

	int size = BUFFER_SIZE;
	int i, j, first_dev, offset;
	for (j = num_disks * raid_num; j < num_disks * raid_num + num_disks; j++){/*first disk for xor*/
		if (j != dev_num){
			first_dev = j;
			break;
		}
	}

	for (i=0; i<=(DEVICE_SIZE/BUFFER_SIZE)+1; i++){/*fix disks for buffer size jumps*/
		offset=size*i;
		if (i==((DEVICE_SIZE/BUFFER_SIZE)+1))/*last bytes*/{
			size=DEVICE_SIZE%BUFFER_SIZE;
			}
		if (repair_with_xor(first_dev, dev_num, offset, size) != -1){/*repair failed*/
			return dev_num;
		}
	}
	return -1;
}

int do_raid50_rw(char *operation, int sector, int count){
	/*read or write to raid50*/
	int i,b,op_failed;
	for (b = 0; b < BUFFER_SIZE; b++){
		keep_buf[b] = buf[b];/*keep new data in keep buffer*/
	}
	for (i = sector; i < sector + count;){
		op_failed=0;
		/*find the real sector for current logical sector*/
		int logic_block = i / SECTOR_PER_BLOCK;
		int ap_jumps = logic_block / (num_disks - 1);
		int sector_jumps = ap_jumps*SECTOR_PER_BLOCK;
		int real_sector = i + sector_jumps;

		/*find the relevant device for current real sector*/
		int block_num = logic_block + ap_jumps;
		int line = block_num / num_dev;/*check parity block*/
		int parity_disk = num_disks - 1 - (line%num_disks);/*parity disk in the raid*/
		int disk = block_num%num_disks;/*num in the raid not in total*/
		if (disk == parity_disk){/*fall on parity so move forward*/
			++disk;
			++block_num;
			real_sector += 2;
		}
		int dev_num = (block_num%num_dev);
		int raid_num=dev_num/num_disks;
		parity_disk = parity_disk + (raid_num*num_disks);/*real parity disk number*/

		/*make sure device did not fail"*/
		if (dev_fd[dev_num] < 0){
			op_failed=1;
		}

		/*find offset of sector inside device*/
		int block_start = real_sector / (num_dev*SECTOR_PER_BLOCK);
		int block_off = real_sector%SECTOR_PER_BLOCK;
		int sector_start = block_start*SECTOR_PER_BLOCK + block_off;
		int offset = sector_start*SECTOR_SIZE;

		/*try to write a few sectors at once*/
		int num_sectors = SECTOR_PER_BLOCK - block_off;
		while (i + num_sectors > sector + count){
			--num_sectors;
		}
		int sector_end = sector_start + num_sectors - 1;
		int size = num_sectors*SECTOR_SIZE;

		/*validate calculations*/
		assert(num_sectors > 0);
		assert(size <= BUFFER_SIZE);
		assert(offset + size <= DEVICE_SIZE);

		printf("Operation on device %d, logic sector %d-%d\n", dev_num, sector_start, sector_end);

		if (!strcmp(operation, "READ")){/*read from current disk all sectors needed from block*/			
			printf("Read operation on disk: %d\n",dev_num);			
			printf("Now accessing disk: %d\n",dev_num);
			if (op_failed==1||(offset != lseek(dev_fd[dev_num], offset, SEEK_SET))||(size != read(dev_fd[dev_num], buf, size))){
				if (read_failed(dev_num, offset, size) != -1){
					return (dev_num);/*read failed*/
				}
			}
			else{
				printf("Reading from disk: %d ended successfully!\n", dev_num);
			}
			int sect,value;
			for (sect = i; sect < i + num_sectors; sect++){
				value= byte_value(i*8);
				printf("sector: %d byte value: %d\n", sect,value);
			}
		}
		else if (!strcmp(operation, "WRITE")){/*write and update parity block*/
			if (write_and_update_parity(dev_num, parity_disk, offset, size) != -1){
				return dev_num;/*updating failed*/
			}		
		}
		i += num_sectors;/*move to next sector*/
	}
	return -1;
}

int do_raid50_kill(int dev_num){
	/*kill selected disk*/
	printf("Now accessing disk: %d\n",dev_num);
	if (close(dev_fd[dev_num]) < 0){
		return dev_num;
	}
	dev_fd[dev_num] = -1;
	return -1;
}

int main(int argc, char** argv){

	assert(argc>2);

	num_disks=(int)((argv[1])[0]-'0');
	num_raids=(int)((argv[2])[0]-'0');	
	num_dev=num_raids*num_disks;
	num_logical_dev=num_raids*(num_disks-1);/*logical devices*/


	assert(argc>2+num_dev);
	
	int _dev_fd[num_dev];
	dev_fd=_dev_fd;

	char line[1024];
	int i;
	
	disks_paths=argv;/*list of disks paths*/
	disks_paths=disks_paths+3;
	
	/*open all devices*/
	for (i=0; i<num_dev; ++i){
		printf("Opening device %d: %s\n", i, disks_paths[i]);
		dev_fd[i]=open(disks_paths[i],O_RDWR);
		/*assert(dev_fd[i]>=0);*/	
	}
	
	
	for (i=0; i<BUFFER_SIZE; ++i){
		buf[i]=0;
		temp_buf[i]=0;
		keep_buf[i]=0;
	}
	
	char operation[20];
	int sector;
	int count;
	int disk_failed;
	
	printf("insert command: operation, sector, count\n");
	while (fgets(line,1024,stdin)!=NULL){
		assert(sscanf(line, "%s %d %d",operation, &sector,&count)==3);
		if (!strcmp(operation,"KILL")){
			if (do_raid50_kill(sector) != -1){
				printf("ERROR!: killing disk %d failed\n", sector);
				break;
			}
			else{
				printf("Disk %d was successfully killed\n", sector);
			}
		}
		else if (!strcmp(operation,"REPAIR")){
			if ((disk_failed=do_raid50_repair(sector))!=-1){
				printf("ERROR! Repairing disk: %d failed!\n",sector);
				break;
			}
			else{
				printf("Repraing disk: %d ended successfully!\n", sector);
			}
		}
		else if (!strcmp(operation,"SETBUF")){
			do_raid50_setbuf(sector);
		}	
		else{
			if ((disk_failed=do_raid50_rw(operation, sector,count))!=-1){
				printf("ERROR!: operation %s on disk %d failed\n",operation, disk_failed);
				break;
			}
		}
		printf("insert command: operation, sector, count\n");
	}

	for (i=0; i<num_dev; i++){
		if (dev_fd[i]>=0){
			assert(!close(dev_fd[i]));		
		}	
	}

	return 1;
}

