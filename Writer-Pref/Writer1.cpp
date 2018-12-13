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
    ofstream myFile;                                                // makes an ifstream object to read from myFile
    myFile.open ("myFile.txt", ios::out | ios::app);              // ::app appends the myFile (new line)
    
    int shmid = shmget(SHM_KEY, 64, 0777|IPC_CREAT);               // 0777 is the permission, IPC_CREAT creates a shared mem block
    int *ptr = (int*)shmat(shmid, 0, 0);                            // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);                  // Creates two semaphores
    int writerCount = 0;

    while(1){
        semop(semid, &psembufW, 1);                                 // lock writer to prevent race conditions
        writerCount++;
        if(writerCount == 1){                                   // if 1st reader
            semop(semid, &psembufT, 1);                         // lock the try semap, 
        }
        semop(semid, &vsembufW, 1);                             // unlock writer to release entry section
        semop(semid, &psembufF, 1);                             // lock resource from other writers
        if(myFile.is_open()){
            cout << "Please enter something\n";
            cin >> op;
            myFile << op;
            myFile.close();
        }
        semop(semid, &vsembufF, 1);                             // unlock file for other writers 
        semop(semid, &vsembufW, 1);                             // lock writer to reserve entry section
        writerCount--;
        if(writerCount == 0){                                   // if 1st reader
            semop(semid, &vsembufT, 1);                         // unlock the try semap, if last writer
        }
        semop(semid, &vsembufW, 1);                             // unlock writer mutex, release exit section
    }
}