#include "unistd.h"
#include "stdio.h"
#include <iostream>
using namespace std;

int main(){
    int processID = fork();
    int counter = 0;
    if(processID == 1){
        for(int i = 0; i < 100; i++)
            cout << "hello from parent, Counter: %d\n", ++counter;

    } else if(processID == 0){
        for(int i = 0; i < 100; i++)
            execl("/home/zenbook/OSFinalY/Lab4", "child.cpp", NULL);
            cout << "hello from child, Counter: %d\n", ++counter;
    }
    cout << "Goodbye";
}
/*
The exec() family of functions replaces the current process image
with a new process image. A process image is a copy of its state at a
given point in time.
execl(const char *path, const char *arg, ...);
*/
