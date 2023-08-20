// The first few routines have been tested in other files
int volatile *const ConsoleWriteSInt = (int *)0x0804;

int volatile *const ConsoleWriteChar = (int *)0x0800;

int volatile *const ConsoleWriteUSInt = (int *)0x0808;

void prints(char * str)
{
    while(*str)
    {
        *((char*) 0x0800) = *(str++);
    }
}


inline int scan_SignedInteger() {
    int result;
    int addr = 0x0816;
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

inline int scan_USIntegerInteger() {
    int result;
    int addr = 0x0816;
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

inline char scan_Char() {
    char result;
    int addr = 0x0812;
    asm volatile("lb %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));
    return result;
}

int main(void)
{
    int a = scan_SignedInteger();
    int b = scan_USIntegerInteger();
    char c = scan_Char();
    
    prints("Signed integer: ");
    *ConsoleWriteSInt = a;
    prints("\nUnsigned integer: ");
    *ConsoleWriteUSInt = b;
    prints("\nCharacter: ");
    *ConsoleWriteChar = c;
    prints("\n");

    return 0;
}
