#include <iostream>
#include <unistd.h>
#include <pthread.h> /* POSIX threads */
using namespace std;

int i = 0;

void *thread_routine1 (void *arg1 ){ // of type void pointer, accepting void pointer
    /* Task 3
    int j;
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
           cout << "Hello, World (thread " << i << ") \n";
           */
    // 3A
    int test = *((int *) arg1); // dereferencing arg as a void
    cout <<"Hello ";
    pthread_exit(NULL);
}

void *thread_routine2 (void *arg2 ){ // second thread function
    cout <<"world \n";
    pthread_exit(NULL);
}

int main ()
{
    /*
    int rc;
    pthread_t thread1 ;
    pthread_t thread2 ;

    cout <<"main process has PID="<< getpid() << endl;
    create thread
   // pthread_create(&thread1, NULL, thread_routine1, NULL);
   // pthread_create(&thread2, NULL, thread_routine2, NULL); // creates second thread
  //  cout <<"Threads are finished \n";


   // rc=pthread_create( &thread1, NULL, &thread_routine1, NULL);
   // if (rc==1) cout <<"Thread creation failed\n";
    */
    /* Task 3
    pthread_t threads[10]; // array of pthreads
    for(i = 0; i < 10; i++){
        pthread_create(&threads[i], NULL, thread_routine1, NULL);
        pthread_join(threads[i], NULL);
    }
    pthread_join(threads[9], NULL);// suspend execution of the calling thread
       */                             // until the target thread terminates
    //Task 3A
    pthread_t thread1;
    pthread_t thread2;

    string t1 = "hello", t2 = "world";
    pthread_create(&thread1, NULL, thread_routine1, t1);
    pthread_create(&thread2, NULL, thread_routine2, t2);
    pthread_exit(NULL);
}

/*
Task1:
2. The order of the threads are printed in wrong order sometimes
    and right at others
3. The order in which the function are printed is irregular, suggests
    two threads.

Task2:


    https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI
*/
