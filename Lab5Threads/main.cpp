#include <iostream>
#include <unistd.h>
#include <pthread.h> /* POSIX threads */
using namespace std;

void *thread_routine1 (void *arg1 ){
    cout <<"Hello ";
    pthread_exit(NULL);
}
void *thread_routine2 (void *arg2 ){ // second thread function
    cout <<"world \n";
    pthread_exit(NULL);
}
int main ()
{
    pthread_t thread1 ;
    pthread_t thread2 ;
    cout <<"main process has PID="<< getpid() << endl;
    /* create thread */
    pthread_create(&thread1, NULL, thread_routine1, NULL);
    pthread_create(&thread2, NULL, thread_routine2, NULL); // creates second thread
    cout <<"Threads are finished \n";
    pthread_exit(NULL);
}

/*
Task1:
2. The order of the threads are printed in wrong order sometimes
    and right at others
3.

    https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI
*/