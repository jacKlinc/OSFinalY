#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <inttypes.h>

#define SHM_KEY 9876 // the shared process id
#define SEMKEY 1234
struct sembuf psembuf, vsembuf; // struct with (p, v) being down, up

int main()
{
    int semid;
    union semun{ // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[256];
    } arg;

    // Program 1: write 1 to all blocks, then 0 to all
    /* create a 256 byte block of shared memory */
    int shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777
    int *ptr = (int*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W
   
    /* semaphore setup */
    semid = semget(SEMKEY, 1, 0777|IPC_CREAT);
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);  /* initialises semaphore 0.  Do this once only  */
    psembuf.sem_num = 0;
    psembuf.sem_op = -1;
    psembuf.sem_flg = SEM_UNDO;
    /* the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    but not performing the corresponding increment the kernel will do it on behalf of the process */

    vsembuf.sem_num = 0;
    vsembuf.sem_op = 1;
    vsembuf.sem_flg = SEM_UNDO;
    /* reserve the shared memory area via the semaphore */
    //semop(semid, &psembuf, 1); // do an up

	while(1){
		for(int i = 0; i < 256; i++){
			semop(semid, &psembuf, 1); // do an up
			printf("Yass %d\n", ptr);
			semop(semid, &vsembuf, 1); // do a down
		}
	}


    /* release the shared memory area via the semaphore */
    //semop(semid, &vsembuf, 1); // do a down

}

/*
The second one should attach to the same area of memory and  repeatedly read 
the block to the screen.  Implement these two programs with and without semaphore protection and observe the difference in operation.  Run the first program as a background process with “&” so as to allow you to later launch the second program from the keyboard.   

semget: nsems is the number of  semaphores in the array.
	flags identifies who has permission to use the semaphores along with a 
	parameter (IPC_CREAT) to optionally create the array if one with this key 		doesn’t already exist.

semop: semops is a pointer to an array of semaphore-operation structures.  
	nosops is the size of this array.  The structure sembuf is declared in sem.h 
	and looks like: struct sembuf{
		 	unsigned short sem_num; /* which semaphore array element to operate on */
		 	short sem_op;           /* the operation to perform 
		 	short sem_flag;
	     		};
	The function returns the value of the last semaphore operated on in the set 		before the current operation was performed  

semctl:
	SETVAL:  Set the value of semval in the semaphore structure to arg.val. 
	SETALL:   Set semvals according to the array pointed to by arg.array. 

	The commands are executed on the semaphore specified by semid and semnum or on 
	a set of semaphores of length semnum.  
	semid is the value returned from the semgetcall. 
*/
