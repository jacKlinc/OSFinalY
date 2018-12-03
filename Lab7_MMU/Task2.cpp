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
            } else if(mes != memory[i].memSize[j]){
                cout << "Page not currently in memory\n"; 
            } else if(mes == memory[i].memSize[j]){
                memory[i].rBit = 1;                 // sets the reference bit
            }
        }
    }
    return 0;
}
/*
Adjust the above program to include a memory map table that holds the information on what
page frames in memory are free, a zero indicates a free frame and a 1 an occupied frame.
Assume that there are 12 page frames in memory.

If a logical address translation results in a page fault then a free frame should be found in the
memory map table, allow for at least one free frame. The memory map table should be
changed and the page table of the process changed to reflect the new situation. The physical
address should then be displayed.
*/
