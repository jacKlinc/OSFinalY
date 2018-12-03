#include <string>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
using namespace std;

struct PageTable {                              // Page tables keep track of VA->PA mappings
	unsigned short pBit;                        // Present bit
	unsigned short rBit;                        // Reference bit
    unsigned short modBit;                      // Modified bit
    unsigned short frameNo;                     // Page in memory it occupies 
    unsigned short memSize[2048];               // The page size is 2kB
}mem;

int main()
{
    struct PageTable memory[5] = {              // 5 pages of memory, we need the Page Table Entry (PTE) to get PA 
        {1, 0, 0, 11},                          // pBit, rBit, modBit, frameNo 
        {0, 0, 0, 0},
        {1, 1, 1, 10},
        {1, 0, 0, 5},
        {1, 1, 0, 7}
    };

    string message;
    int mes;
    for(int i = 0; i < 5; i++){
        cout << "Enter valid address\n";
        cin >> message;
        mes = stoi(message);
        for(int j = 0; j < 2048; j++){
            if(message.length() > 32){                                   // how is addr illegal
                cout << "ADDR illegal\n";
                exit(0);
            } 
            if(mes != memory[i].memSize[j]){
                cout << "Page not currently in memory\n"; 
            } else if(mes == memory[i].memSize[j]){
                memory[i].rBit = 1;                 // sets the reference bit
            }
        }
    }
    return 0;
}
/*
Write code that allows a user to enter a logical address and then takes the
following action:
If the address is illegal an illegal address message is printed and the program is
terminated.
If the address translates to a page that is not currently in memory then a page fault
message should be generated, note a page is 2048 bytes long.
If the address translates to a page currently in memory the reference bit should be set,
the page frame that the page occupies printed and the physical address calculated and
outputted to the screen.*/
