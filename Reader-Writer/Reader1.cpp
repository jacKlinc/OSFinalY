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
struct sembuf psembuf, vsembuf; // struct with (p, v) being down, up
// ************* Init semaphores once, in one file ***************

int main()
{
    union semun{                // like struct but all members share the same location
        int val;
        struct semid_ds *buf;
        ushort myArray[0];
    } arg;

    
    string op;
    /*ifstream myfile;
    myfile.open ("myFile.txt");
    getline(myfile, op);
    cout << op;
    myfile.close();
    */
    ofstream file;
    file.open ("myFile.txt", ios::out | ios::app); // ::app appends the file (new line)
    cout << op;
    file << "Writing this to a file.\n";
    file.close();
    /*
    int shmid = shmget(SHM_KEY, 256, 0777|IPC_CREAT); // 0777
    int *ptr = (int*)shmat(shmid, 0, 0); // starting addr of 1, 0 is full R/W
    int semid = semget(SEMKEY, 1, 0777|IPC_CREAT); // semaphore setup 
    int readerCount = 0;

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);  // initialises semaphore 0.  Do this once only  
    psembuf.sem_num = 0;
    psembuf.sem_op = -1;
    psembuf.sem_flg = SEM_UNDO;
    // the purpose of the SEM_UNDO is to ensure that if a process exits after decrementing a semaphore
    //but not performing the corresponding increment the kernel will do it on behalf of the process 

    vsembuf.sem_num = 0;
    vsembuf.sem_op = 1;
    vsembuf.sem_flg = SEM_UNDO;

	while(1){
        semop(semid, &psembuf, 1); // do an up
		for(int i = 0; i <= 256; i++) {
			cout << *(ptr + i);
        }
        readerCount++;
        semop(semid, &vsembuf, 1); // do a down
        cout << "256\n\n";
	}
    */
}


/*semid=semget(SEMKEY,2,0777|IPC_CREAT);
the 2 in the system call create two semaphore.
2. To initialise a particular semaphoreAssignment 2018 Due 10 th November
semctl(semid,0,SETVAL,1);

the 0 in the system call refers to the 1 st semaphore in the group of two. Each
semaphore needs to be initialized to the correct value. Only one process should do
this.

3. the operations that can be performed on a semaphore must be defined for each
semaphore i.e. we defined a number of variables of type sembufs truct sembuf 
vsembuf, psembuf, v1sembuf, p1sembuf; then we initialise the structure members 

psembuf.sem_num=0;
psembuf.sem_op=-1;
psembuf.sem_flg=SEM_UNDO;
vsembuf.sem_num=0;
vsembuf.sem_op=1;
vsembuf.sem_flg=SEM_UNDO;
p1sembuf.sem_num=1;
p1sembuf.sem_op=-1;
p1sembuf.sem_flg=SEM_UNDO;
v1sembuf.sem_num=1;
v1sembuf.sem_op=1;
v1sembuf.sem_flg=SEM_UNDO;

Then to perform a down or wait operation on semaphore 1
semop(semid,&p1sembuf,1)
or an up or signal on semaphore 1
semop(semid,&v1sembuf,1)
*/