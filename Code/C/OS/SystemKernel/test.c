#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char **argv) {
  
  int dev_1, dev_2;
  dev_1 = open("/dev/simple_char_dev", O_RDONLY);
  assert(check > 0);
  dev_2 = open ("/dev/simple_char_dev2", O_RDONLY);
  if(dev_2 < 0){
  	printf("Error : %s \n", strerror(errno));
  }

  close(dev_1);
  close(dev_2);  
  return;
}

/****************************************

1.what are we seeing ? why ?
answer : we are reading from two devices, one at a time(open read close on first than open read close on second - again and again),
		 and each time we are getting different letters(the netx letter of the name(mod)).
		 The reason for this is that both devices are using the same device driver, and so they share the same buffer.that leads to advencing the one buffer in each read call,
		 and so when the next device calls for read, it gets the letter that comes after the letter that the previous device got.



2.while runing prog test.c i tried to open two device that uses the same device driver and recieved the error : "Error: Device or resurce busy".
 Reason :   we cannot have more than one device, that uses the same device driver, open at all times. 
			while trying to open the second device before closing the first one we get : "Error: Device or resurce busy"
			That means that concurrent access is restricted.
			the lock mechanism in the device driver prevents accessing from two different devices that uses it.

*******************************************/

