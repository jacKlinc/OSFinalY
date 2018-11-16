#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#define SHM_KEY 9876 // the shared process id

using namespace std;

int main()
{
	char *shm, *s;
    int shmid, *ptr;
    //const void *shmaddr;
    shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777 is access, creates 256B block shm 
    ptr = (int*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W permission
  
    /*
     * Now read what the server put in the memory.
     */
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';

    exit(0);
}

/*
second process should attach to the same area of memory. 
It monitors the shared memory block. 
Each time a string is written to the block, it reads the data and sends it 
to the screen. 

The second process will need to know when the buffer has new data in it. 
The first location in shared memory could be used as a flag which is set by
process 1 when it has written new data to the shared area and 
reset by process 2 when it has read new data from the shared area. 

Activate two Terminal windows on your desktop. 
Run the first process in 1 Terminal window. 
Run the 2 nd process in the other window. 
Now as you type in 1 window you should see the text also appearing in 
the other window.
*/