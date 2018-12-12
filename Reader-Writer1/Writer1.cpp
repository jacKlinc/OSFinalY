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
    ofstream myFile;                                                // makes an ifstream object to read from myFile
    myFile.open ("myFile.txt", ios::out | ios::app);              // ::app appends the myFile (new line)
    
    int shmid = shmget(SHM_KEY, 64, 0777|IPC_CREAT);               // 0777 is the permission, IPC_CREAT creates a shared mem block
    int *ptr = (int*)shmat(shmid, 0, 0);                            // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 2, 0777|IPC_CREAT);                  // Creates two semaphores
    int readerCount = 0;

    while(1){
        semop(semid, &psembuf1, 1);                                 // lock file for writer
        if(myFile.is_open()){
            cout << "Please enter something\n";
            cin >> op;
            myFile << op;
            myFile.close();
        }
        semop(semid, &vsembuf1, 1);                                 // unlock file for writers if no readers request
    }
}