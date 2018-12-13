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

    string op;
    ifstream myFile;                                 // makes an ifstream object to read from myFile
    
    int shmid = shmget(SHM_KEY, 64, 0777|IPC_CREAT);// 0777 is the permission, IPC_CREAT creates a shared mem block
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);  // Creates two semaphores
    int *readerCount = (int*)shmat(shmid, 0, 0);    // starting addr of 1, 0 is full R/W
    int pause;
    *readerCount = 0;

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
    semctl(semid, 0, SETVAL, arg);                  // initialises semaphore 0.  Do this once only  
    semctl(semid, 1, SETVAL, arg);

    while(1){
        cout << "Reader1:\n";
        pause = getchar();
        semop(semid, &psembuf, 1);                  // lock reader mutex 
        *readerCount++;
        if(*readerCount == 1){                      // is this first reader
            cout << "Locking reader from writers\n";
            semop(semid, &psembuf1, 1);             // lock resource from writers if 1st reader
        }
        semop(semid, &vsembuf, 1);                  // unlock reader mutex
        myFile.open ("myFile.txt", ios::out | ios::app); // ::app appends the myFile (new line)
        if(myFile.is_open()){        
            while(getline(myFile, op)){
                cout << op << endl;
            }
            myFile.close();
        }
        semop(semid, &psembuf, 1);                  // lock reader mutex
        *readerCount--;                             // decrements readerCount                             
        if(*readerCount == 0) {                     // is this the last reader
            semop(semid, &vsembuf1, 1);             // unlock resource mutex for writers
            cout << "Locking reader mutex for readers\n";
        }
        semop(semid, &vsembuf, 1);                  // unlock reader mutex (release)
        semop(semid, &vsembuf1, 1);                 // unlock resource mutex
    }  
    return 0;  
}

    // the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    //but not performing the corresponding increment the kernel will do it on behalf of the process 