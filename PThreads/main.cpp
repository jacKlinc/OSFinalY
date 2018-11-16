#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h> /* POSIX threads */
using namespace std;

//int i = 0;
string gMessage = "C++ is bollox";

/*
void *thread_routine1(void *arg1){ // of type void pointer, accepting void pointer
    /* Task 3
    int j;
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
           cout << "Hello, World (thread " << i << ") \n";

    // Task 3B
    cout << *static_cast<string*>(arg1) << endl; // static cast: converts types
    pthread_exit(NULL);
    return nullptr;
}

void *thread_routine2 (void *arg2 ){ // second thread function
    // Task 3B
    cout << *static_cast<string*>(arg2) << endl; // static cast: converts types
    pthread_exit(NULL);
    return nullptr;
}
*/
void *thread_routine3 (void *arg3 ){ // asks for text to be inputted, nothing passed
    // Task 4
    cout << "\nPlease enter something \n";
    cin >> gMessage;
    cout << endl;
    //cout << *static_cast<void*>(gMessage) << endl; // static cast: converts type
    pthread_exit(NULL);
    return nullptr;
}

void *thread_routine4 (void *arg4 ){ // prints the global var
    // Task 4
    cout << *static_cast<string*>(arg4) << endl; // static cast: converts type
    pthread_exit(NULL);
    return nullptr;
}

int main ()
{
    /*
    // Task 2
    int rc;

    cout <<"main process has PID="<< getpid() << endl;
    create thread
   // pthread_create(&thread1, NULL, thread_routine1, NULL);
   // pthread_create(&thread2, NULL, thread_routine2, NULL); // creates second thread
  //  cout <<"Threads are finished \n";


   // rc=pthread_create( &thread1, NULL, &thread_routine1, NULL);
   // if (rc==1) cout <<"Thread creation        failed\n";
    */
    /* Task 3
    pthread_t threads[10]; // array of pthreads
    for(i = 0; i < 10; i++){
        pthread_create(&threads[i], NULL, thread_routine1, NULL);
        pthread_join(threads[i], NULL);
    }
    pthread_join(threads[9], NULL);// suspend execution of the calling thread
       */                             // until the target thread terminates
    /*Task 3B

    pthread_t thread1;
    pthread_t thread2;

    string t1 = "hello", t2 = "world";
    pthread_create(&thread1, NULL, thread_routine1, &t1); // pass the address of t1
    pthread_create(&thread2, NULL, thread_routine2, &t2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Task 4
    pthread_t thread3;
    pthread_t thread4;
    while(1) {// indefinite loop
        pthread_create(&thread3, NULL, thread_routine3, NULL);
        pthread_create(&thread4, NULL, thread_routine4, &gMessage);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
    }*/
    // Task 5: same as previous task but with no join
    pthread_t thread3;
    pthread_t thread4;
    while(1) {// indefinite loop
        pthread_create(&thread3, NULL, thread_routine3, NULL);
        pthread_create(&thread4, NULL, thread_routine4, &gMessage);
    }
    return 0;
}

/*
Task1:
2. The order of the threads are printed in wrong order sometimes
    and right at others
3. The order in which the function are printed is irregular, suggests
    two threads.

Task5:
    Prints out the same strings but in completely random order and without
    waiting for user input.
*/
