#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY 9876 // the shared process id

using namespace std;

int main()
{
    int shmid, *ptr;
    //const void *shmaddr;
    shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777 is access, creates 256B block shm 
    ptr = (int*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W permission
    cout << "Enter come characters \n";
}