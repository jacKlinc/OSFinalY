#include "unistd.h"
#include "stdio.h"

int main(){
    int processID = fork();
    int counter = 0;
    if(processID == 1){
        for(int i = 0; i < 100; i++)
            printf("hello from parent, Counter: %d\n", ++counter);

    } else if(processID == 0){
        for(int i = 0; i < 100; i++)
            printf("hello from child, Counter: %d\n", ++counter);
            execl();
    }
    printf("----End------");
}
