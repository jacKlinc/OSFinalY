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
struct sembuf vsembuf, psembuf, vsembuf1, psembuf1; // struct with (p, v) being down, up
// ************* Init semaphores once, in one myFile ***************

int main()
{
    union semun{                                                    // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[0];
    } arg;
    /*ifstream mymyFile;
    mymyFile.open ("mymyFile.txt");
    getline(mymyFile, op);
    cout << op;
    mymyFile.close();  */
    string op;
    ifstream myFile;                                                // makes an ifstream object to read from myFile
    myFile.open ("myFile.txt", ios::out | ios::app);              // ::app appends the myFile (new line)
    
    int shmid = shmget(SHM_KEY, 64, 0777|IPC_CREAT);               // 0777 is the permission, IPC_CREAT creates a shared mem block
    int *ptr = (int*)shmat(shmid, 0, 0);                            // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);                  // Creates two semaphores
    int readerCount = 0;

    psembuf.sem_num=0;                      // init mutex members
    psembuf.sem_op=-1;
    psembuf.sem_flg=SEM_UNDO;
    vsembuf.sem_num=0;
    vsembuf.sem_op=1;
    vsembuf.sem_flg=SEM_UNDO;

    psembuf1.sem_num=1;                     // resource
    psembuf1.sem_op=-1;
    psembuf1.sem_flg=SEM_UNDO;
    vsembuf1.sem_num=1;
    vsembuf1.sem_op=1;
    vsembuf1.sem_flg=SEM_UNDO;

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);                                  // initialises semaphore 0.  Do this once only  
    // the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    //but not performing the corresponding increment the kernel will do it on behalf of the process 
    while(1){
        semop(semid, &psembuf, 1);                  // lock reader mutex 
        readerCount++;
        cout << op << endl;                         // reads
        if(readerCount == 1)                        // is this first reader
            semop(semid, &psembuf1, 1);             // lock resource from writers if 1st reader
                                                    
        if(myFile.is_open()){        
            while(getline(myFile, op)){
                semop(semid, &vsembuf, 1);          // unlock reader mutex
                readerCount--;                      // decrements readerCount                             
                if(readerCount == 0)                // is this the last reader
                    semop(semid, &vsembuf1, 1);     // lock reader mutex for other readers
                semop(semid, &vsembuf, 1);          // lock reader mutex (release)
            }
        }
    }    
    /*
    semop(semid, &psembuf1, 1);                // down or wait
    semop(semid, &vsembuf1, 1);                // up or signal
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
psembuf, vsembuf, vsembuf1, psembuf1; then we initialise the structure members 

vsembuf.sem_num=0;
vsembuf.sem_op=-1;
vsembuf.sem_flg=SEM_UNDO;
psembuf.sem_num=0;
psembuf.sem_op=1;
psembuf.sem_flg=SEM_UNDO;
psembuf1.sem_num=1;
psembuf1.sem_op=-1;
psembuf1.sem_flg=SEM_UNDO;
vsembuf1.sem_num=1;
vsembuf1.sem_op=1;
vsembuf1.sem_flg=SEM_UNDO;

Then to perform a down or wait operation on semaphore 1
semop(semid,&psembuf1,1)
or an up or signal on semaphore 1
semop(semid,&vsembuf1,1)
*/