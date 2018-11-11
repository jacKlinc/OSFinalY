#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY 9876 // the shared process id

using namespace std;

int main()
{
    int shmid, *ptr;
    char *charArray; // array of characters
    shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777 is access, creates 256B block shm 
    charArray = (char*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W permission
    
    for(int i = 0; i < 50; i++){
    	cout << "Enter some characters \n";
    	cin >> charArray[i];
    	charArray[i] = i;
    } 
    return 0;
}

/*
creates shared memory and attach to it. 
It then reads characters from the keyboard and stores them in a local 
array as a character string. 
Each time a string is placed in the array, the process writes the array 
to the shared memory area.
It then waits for the next characters to be typed and repeats etc.
*/