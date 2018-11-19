#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_KEY 9876                    // the shared process id
#define SEMKEY 1234
struct sembuf psembuf, vsembuf;         // struct with (p, v) being down, up

int main()
{
    
    int semid;
    union semun{                        // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[256];
    } arg;

    int shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777-permission, 256B memory
    int *ptr = (int*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W
    
    semid = semget(SEMKEY, 1, 0777|IPC_CREAT); // semaphore setup 
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);      // initialises semaphore 0.  Do this once only  
    psembuf.sem_num = 0;
    psembuf.sem_op = -1;
    psembuf.sem_flg = SEM_UNDO;
    // the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    //but not performing the corresponding increment the kernel will do it on behalf of the process 

    vsembuf.sem_num = 0;
    vsembuf.sem_op = 1;
    vsembuf.sem_flg = SEM_UNDO;

    while(1){
        for(int i = 0; i <= 256; i++) {   // 1 for each of the 256
        	semop(semid, &psembuf, 1);  // do an up, reserve the shared memory area via the semaphore 
            *(ptr + i) = 1;
            semop(semid, &vsembuf, 1);  // do a down, release the shared memory area via the semaphore
        }
        for(int i = 0; i <= 256; i++) {   // 0
            semop(semid, &psembuf, 1);  // do an up
        	*(ptr + i) = 0;
            semop(semid, &vsembuf, 1);  // do a down
        }
    }
}

/*
The first one should create a 256 byte block of shared memory and attach to it. 
It should then write 0 to all the bytes in the block, then write 1 to all bytes and 
then 0, etc. repeatedly.  

semget: nsems is the number of  semaphores in the array.
	flags identifies who has permission to use the semaphores along with a 
	parameter (IPC_CREAT) to optionally create the array if one with this key doesn’t already exist.

semop: semops is a pointer to an array of semaphore-operation structures.  
	nosops is the size of this array.  The structure sembuf is declared in sem.h 
	and looks like: 
    struct sembuf{
	 	unsigned short sem_num; /* which semaphore array element to operate on 
	 	short sem_op;           /* the operation to perform 
	 	short sem_flag;
	};
	The function returns the value of the last semaphore operated on in the set before the current operation was performed  

semctl:
	SETVAL:  Set the value of semval in the semaphore structure to arg.val. 
	SETALL:   Set semvals according to the array pointed to by arg.array. 

	The commands are executed on the semaphore specified by semid and semnum or on 
	a set of semaphores of length semnum.  
	semid is the value returned from the semgetcall. 
*/
