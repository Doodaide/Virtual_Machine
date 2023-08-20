#include <stdint.h>

inline int writeToHeap(unsigned char size)
{
    unsigned char toStore = size;
    int addr = 0x0830;

    asm volatile("sb %0, 0(%1)" :: "r"(toStore), "r"(addr));
    return 0;
}   

inline int virtMemStore(int addr, unsigned char toStore)
{
    asm volatile("sb %0, 0(%1)" :: "r"(toStore), "r"(addr));
}

inline int loadMem(int result, int addr)
{
    asm volatile("lb %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));
}

void prints(char * str)
{
    while(*str)
    {
        *((char*) 0x0800) = *(str++);
    }
}


char volatile *const ConsoleWriteCharacter = (char *)0x0800;
int main()
{
    unsigned char val;
    prints("HeapBank Test!\n");
    writeToHeap(65);
    //virtMemStore(0x0824, 0x0);
    virtMemStore(0xb700, 'A'); // Write to 0 and 1
    asm volatile("lb %0, 0(x28)" : "=r"(val));
    prints("\n**----------------**\n");
    prints("Output: ");
    *ConsoleWriteCharacter = val;
    prints("\n**----------------**\n");
    val = 0;
    writeToHeap(10);
    virtMemStore(0xb780, 'B');
    asm volatile("lb %0, 0(x28)" : "=r"(val));
    prints("\n**----------------**\n");
    prints("Output: ");
    *ConsoleWriteCharacter = val;
    prints("\n**----------------**\n");
    val = 0;
    writeToHeap(3);
    virtMemStore(0xb7C0, 'C');
    asm volatile("lb %0, 0(x28)" : "=r"(val));
    prints("\n**----------------**\n");
    prints("Output: ");
    *ConsoleWriteCharacter = val;
    prints("\n**----------------**\n");

    /*
    Test a new allocation x
    Test free (deallocate 1st assigned, reassign it to 5, then try again)
    */
    prints("\n**----------------**\n");
    prints("FreeTest: ");
    prints("\n**----------------**\n");
    //virtMemStore(0x0834, 0xb700); // Free
    asm volatile("sw %0, 0(%1)" :: "r"(0xb700), "r"(0x0834));


    asm volatile("lb %0, 0(x28)" : "=r"(val));
    prints("\n**----------------**\n");
    prints("Output: ");
    *ConsoleWriteCharacter = val;
    prints("\n**----------------**\n");


    // Invalid address
    asm volatile("sw %0, 0(%1)" :: "r"(0xb800), "r"(0x0834));
    
    return 0;
}