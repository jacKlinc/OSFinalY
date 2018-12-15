#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <inttypes.h>
using namespace std;

#define SHM_KEY 9876                                    // the shared process id
#define SEMKEY  1234
struct sembuf vsembufR, psembufR, vsembufW, psembufW; // reader and writer semaphores
struct sembuf vsembufF, psembufF, vsembufT, psembufT; // resource (file) and read try semaphores

int main()
{
    union semun{                                      // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[0];
    } arg;

    string op;
    ifstream myFile;                                 // makes an ifstream object to read from myFile
    
    int shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT);// 0777 is the permission, IPC_CREAT creates a shared mem block
    int *readerCount = (int*)shmat(shmid, 0, 0);    // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);  // Creates two semaphores
    int pause;
    *readerCount = 0;

    psembufR.sem_num=0;                          // init reader mutex members
    psembufR.sem_op=-1;
    psembufR.sem_flg=SEM_UNDO;
    vsembufR.sem_num=0;
    vsembufR.sem_op=1;
    vsembufR.sem_flg=SEM_UNDO;

    psembufF.sem_num=1;                          // resource
    psembufF.sem_op=-1;
    psembufF.sem_flg=SEM_UNDO;
    vsembufF.sem_num=1;
    vsembufF.sem_op=1;
    vsembufF.sem_flg=SEM_UNDO;

    psembufW.sem_num=0;                         // writer
    psembufW.sem_op=-1;
    psembufW.sem_flg=SEM_UNDO;
    vsembufW.sem_num=0;
    vsembufW.sem_op=1;
    vsembufW.sem_flg=SEM_UNDO;

    psembufT.sem_num=1;                         // try
    psembufT.sem_op=-1;
    psembufT.sem_flg=SEM_UNDO;
    vsembufT.sem_num=1;
    vsembufT.sem_op=1;
    vsembufT.sem_flg=SEM_UNDO;

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);              // initialises semaphore 0.  Do this once only  
    semctl(semid, 1, SETVAL, arg);
    
    while(1){
        cout << "\nReader2: \n";
        pause = getchar();                      // prevent errors
        semop(semid, &psembufT, 1);             // lock reader try mutex (trying to enter)
        semop(semid, &psembufR, 1);             // lock reader mutex (avoid race condition)    
        *readerCount++;
        if(*readerCount == 1)                   // is this first reader
            semop(semid, &psembufF, 1);         // lock resource from writers if 1st reader
        semop(semid, &vsembufR, 1);             // unlock reader mutex (for other readers)
        semop(semid, &vsembufT, 1);             // unlock try mutex (done accessing file)
        // Critical Section
        myFile.open ("myFile.txt", ios::out | ios::app); // ::app appends the myFile (new line)                                           
        if(myFile.is_open()){        
            while(getline(myFile, op)){
                cout << op;             // reads
            }
            myFile.close();
        }
        semop(semid, &psembufR, 1);             // lock reader mutex (avoid race)
        *readerCount--;
        if(*readerCount == 0)                   // is this the last reader
            semop(semid, &vsembufF, 1);         // unlock resource
        semop(semid, &vsembufR, 1);             // unlock reader mutex
    }    
}