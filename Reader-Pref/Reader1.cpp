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
struct sembuf vsembufR, psembufR, vsembufF, psembufF; // Reader Mutex and Resource Mutex

int main()
{
    union semun{                                    // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[0];
    } arg;

    string op;
    ifstream myFile;                                 // makes an ifstream object to read from myFile

    int shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT);// 0777 is the permission, IPC_CREAT creates a shared mem block
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);  // Creates two semaphores
    int *readerCount = (int*)shmat(shmid, 0, 0);    // starting addr of 1, 0 is full R/W
    int pause;                                      // used as later for pausing program
    *readerCount = 0;

    psembufR.sem_num=0;                              // init mutex members
    psembufR.sem_op=-1;                              // what value to use with semop
    psembufR.sem_flg=SEM_UNDO;                       // SEM_UNDO makes kernel increment for process after decrementing semap
    vsembufR.sem_num=0;
    vsembufR.sem_op=1;
    vsembufR.sem_flg=SEM_UNDO;

    psembufF.sem_num=1;                             // resource
    psembufF.sem_op=-1;
    psembufF.sem_flg=SEM_UNDO;
    vsembufF.sem_num=1;
    vsembufF.sem_op=1;
    vsembufF.sem_flg=SEM_UNDO;

    arg.val = 1;                                    // sets to binary semap
    semctl(semid, 0, SETVAL, arg);                  // initialises semaphore 0.  Do this once only  
    semctl(semid, 1, SETVAL, arg);

while(1){
    cout << "Reader1:\n";
    pause = getchar();                          // pause prevents error
    semop(semid, &psembufR, 1);                  // lock reader mutex
    *readerCount++;
    if(*readerCount == 1){                      // is this first reader
        cout << "Locking reader from writers\n";
        semop(semid, &psembufF, 1);             // lock resource from writers if 1st reader
    }
    semop(semid, &vsembufR, 1);                  // unlock reader mutex
    // Critical Section
    myFile.open ("myFile.txt", ios::out | ios::app); // ::app appends the myFile (new line)
    if(myFile.is_open()){        
        while(getline(myFile, op)){
            cout << op << endl;
        }
        myFile.close();
    }
    semop(semid, &psembufR, 1);                  // lock reader mutex, can use readerCount
    *readerCount--;                                                         
    if(*readerCount == 0) {                     // is this the last reader
        semop(semid, &vsembufF, 1);             // unlock resource mutex for writers
        cout << "Locking reader mutex for readers\n";
    }
    semop(semid, &vsembufR, 1);                  // unlock reader mutex (release)
    semop(semid, &vsembufF, 1);                 // unlock resource mutex
}  
    return 0;  
}