#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <inttypes.h>
using namespace std;

#define SHM_KEY 9876            // the shared process id
#define SEMKEY  1234
struct sembuf vsembufR, psembufR, vsembufW, psembufW;                 // reader and writer semaphores
struct sembuf vsembufF, psembufF, vsembufT, psembufT;                 // resource (file) and read try semaphores
// ************* Init semaphores once, in one myFile ***************

int main()
{
    union semun{                                                    // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[0];
    } arg;

    string op;
    ifstream myFile;                                                // makes an ifstream object to read from myFile
    myFile.open ("myFile.txt", ios::out | ios::app);              // ::app appends the myFile (new line)
    
    int shmid = shmget(SHM_KEY, 64, 0777|IPC_CREAT);               // 0777 is the permission, IPC_CREAT creates a shared mem block
    int *ptr = (int*)shmat(shmid, 0, 0);                            // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);                  // Creates two semaphores
    int readerCount = 0;

    psembufR.sem_num=0;                      // init reader mutex members
    psembufR.sem_op=-1;
    psembufR.sem_flg=SEM_UNDO;
    vsembufR.sem_num=0;
    vsembufR.sem_op=1;
    vsembufR.sem_flg=SEM_UNDO;

    psembufF.sem_num=1;                     // resource
    psembufF.sem_op=-1;
    psembufF.sem_flg=SEM_UNDO;
    vsembufF.sem_num=1;
    vsembufF.sem_op=1;
    vsembufF.sem_flg=SEM_UNDO;

    psembufW.sem_num=0;                      // writer
    psembufW.sem_op=-1;
    psembufW.sem_flg=SEM_UNDO;
    vsembufW.sem_num=0;
    vsembufW.sem_op=1;
    vsembufW.sem_flg=SEM_UNDO;

    psembufT.sem_num=1;                     // try
    psembufT.sem_op=-1;
    psembufT.sem_flg=SEM_UNDO;
    vsembufT.sem_num=1;
    vsembufT.sem_op=1;
    vsembufT.sem_flg=SEM_UNDO;

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);                                  // initialises semaphore 0.  Do this once only  
    // the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    //but not performing the corresponding increment the kernel will do it on behalf of the process 
    while(1){
        semop(semid, &psembufR, 1);                 // lock reader mutex 
        semop(semid, &psembufR, 1);                 // lock reader mutex 
        readerCount++;
        cout << op << endl;                         // reads
        if(readerCount == 1)                        // is this first reader
            semop(semid, &psembufF, 1);             // lock resource from writers if 1st reader
                                                    
        if(myFile.is_open()){        
            while(getline(myFile, op)){
                semop(semid, &vsembufR, 1);          // unlock reader mutex
                readerCount--;                      // decrements readerCount                             
                if(readerCount == 0)                // is this the last reader
                    semop(semid, &vsembufF, 1);     // lock reader mutex for other readers
                semop(semid, &vsembufR, 1);          // lock reader mutex (release)
            }
        }
    }    
    /*
    semop(semid, &psembufF, 1);                // down or wait
    semop(semid, &vsembufF, 1);                // up or signal
    */
}


/*
semid=semget(SEMKEY,2,0777|IPC_CREAT);
the 2 in the system call create two semaphore.
2. To initialise a particular semaphoreAssignment 2018 Due 10 th November
semctl(semid,0,SETVAL,1);

the 0 in the system call refers to the 1 st semaphore in the group of two. Each
semaphore needs to be initialized to the correct value. Only one process should do
this.

3. the operations that can be performed on a semaphore must be defined for each
semaphore i.e. we defined a number of variables of type sembufs truct sembuf 
psembufR, vsembufR, vsembufF, psembufF; then we initialise the structure members 

vsembufR.sem_num=0;
vsembufR.sem_op=-1;
vsembufR.sem_flg=SEM_UNDO;
psembufR.sem_num=0;
psembufR.sem_op=1;
psembufR.sem_flg=SEM_UNDO;
psembufF.sem_num=1;
psembufF.sem_op=-1;
psembufF.sem_flg=SEM_UNDO;
vsembufF.sem_num=1;
vsembufF.sem_op=1;
vsembufF.sem_flg=SEM_UNDO;

Then to perform a down or wait operation on semaphore 1
semop(semid,&psembufF,1)
or an up or signal on semaphore 1
semop(semid,&vsembufF,1)
*/