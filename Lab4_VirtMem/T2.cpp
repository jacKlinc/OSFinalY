#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY 9876 // the shared process id

using namespace std;

int main()
{
    int shmid, *ptr2;
    //const void *shmaddr;
    /* create a 256 byte block of shared memory */
    shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777 is access permissions
    /* attach the process to the shared memory area */

    // Program 2: attach to the same area of memory and write a zero into the first location causing the
    //            first process to terminate
    ptr2 = (int*)shmat(shmid, 0, 0); // accesses the same piece of memory
    *ptr2 = 0; // 

    return 0;
}

/*
shmget: returns shared mem id (segment)
        SHM_KEY is rand number chosen by user
        size is mem size
        flags - who has persmission, IPC_CREAT additonal to
                create new shared mem area

shmat:  returns value from shmget call (addr of attached shm segment)
        shmaddr - if zero is first available virt adr
        flags - whether its RD only & if kernel should round
                off user specified addr

After creating a piece of shared memory you need to attach to
it

IPC_CREAT-creates a new segment.
*/
