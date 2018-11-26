#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

struct pageTable {
	unsigned short pBit;        // Present bit
	unsigned short rBit;        // Reference bit
    unsigned short modBit;      // Modified bit
    unsigned short frameNo;     // Frame number
    unsigned short memSize[2048];
};

int main()
{
    pageTable memory[5] = {
        {1,0,0,11}
        {0,0,0,0}
        {1,1,1,10}
        {1,0,0,5}
        {1,1,0,7}};
    }
    unsigned short pres, ref, mod, frameN;
    cout << "Enter Address\n";
    cin >> message;
    if(addr){
        cout<<"ADDR illegal"
    }
    else if()
    return 0;
}
