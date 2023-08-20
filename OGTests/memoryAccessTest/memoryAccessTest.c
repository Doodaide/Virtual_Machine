void prints(char * str)
{
    while(*str)
    {
        *((char*) 0x0800) = *(str++);
    }
}

inline int lhTest(int address)
{
    int result = 10;
    asm volatile("lh %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(address));
    return result;
}

inline int shTest(int address)
{ 
    int toStore = 0xAF;
    asm volatile("sb %0, 0(%1)" :: "r"(toStore), "r"(address));
    return 0;

}

int volatile *const ConsoleWriteUSInt = (int *)0x0808;

int main()
{
    int res;
    int addr = 0x404;
    prints("\n--------------------\n");
    prints("Storing half\nExpected: af");
    shTest(addr);
    prints("\n--------------------\n");
    prints("Loading half\nExpected: ");
    res = lhTest(addr);
    *ConsoleWriteUSInt = res;

    

    prints("\n");
    return 0;
}
