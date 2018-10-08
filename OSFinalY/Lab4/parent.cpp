#include "unistd.h"
#include "stdio.h"
#include <iostream>
using namespace std;

int main(){
    pid_t processID;
    processID = fork();
    int counter = 0;
    if(processID != 0){
        for(int i = 0; i < 10; i++)
            cout << "hello from parent, Counter: "<< ++counter <<endl;
    } else {
        for(int i = 0; i < 10; i++){
	    cout << "hello from child, Counter: " << ++counter <<endl;
            execl("/bin/ls", "ls", "-a", NULL);
    	}
    }
    cout << "Goodbye \n";
}
/*
The exec() family of functions replaces the current process image
with a new process image. A process image is a copy of its state at a
given point in time.
execl(const char *path, const char *arg, ...);
*/
