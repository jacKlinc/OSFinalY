#include <unistd.h>
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
            cout << "hello from child, Counter: %d\n", ++counter;
            execl();
    }
    cout << "----End------";
}
