void prints(char * str)
{
    while(*str)
    {
        *((char*) 0x0800) = *(str++);
    }
}


inline int scan_char() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

inline int addTest(int a, int b)
{
    int result;
    asm volatile("add %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int addiTest(int a, int b)
{
    int result;
    asm volatile("addi %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "i" (b));
    return result;
}

inline int subTest(int a, int b)
{
    int result;
    asm volatile("sub %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int xorTest(int a , int b)
{
    int result;
    asm volatile("xor %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int xoriTest(int a, int b)
{
    int result;
    asm volatile("xori %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "i" (b));
    return result;
}

inline int orTest(int a, int b)
{
    int result;
    asm volatile("or %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int oriTest(int a, int b)
{
    int result;
    asm volatile("ori %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int andTest(int a, int b)
{
    int result;
    asm volatile("and %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int andiTest(int a, int b)
{
    int result;
    asm volatile("andi %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "i" (b));
    return result;
}

inline int sllTest(int a, int b)
{
    int result;
    asm volatile("sll %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int sraTest(int a, int b)
{
    int result;
    asm volatile("sra %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

inline int srlTest(int a, int b)
{
    int result;
    asm volatile("srl %[result], %[a], %[b]\n\t" : [result] "=r" (result): [a] "r" (a), [b] "r" (b));
    return result;
}

int volatile *const ConsoleWriteSInt = (int *)0x0804;
int main() {
    int count = 0;
    int sum = 0;
    int num;
    
    int a = scan_char();
    int b = scan_char();
    prints("\n--------------------\n");
    prints("Addition\nExpected: ");
    *ConsoleWriteSInt = (a + b);
    prints("\nActual: ");
    *ConsoleWriteSInt = addTest(a, b);
    
    prints("\n--------------------\n");
    prints("AdditionImmediate\nExpected: ");
    *ConsoleWriteSInt = (a + 20);
    prints("\nActual: ");
    *ConsoleWriteSInt = addiTest(a, 20);


    prints("\n--------------------\n");
    prints("\nSubtraction\nExpected: ");
    *ConsoleWriteSInt = a - b;
    prints("\nActual: ");
    *ConsoleWriteSInt = subTest(a, b);

    prints("\n--------------------\n");
    prints("\nxor\nExpected: ");
    *ConsoleWriteSInt = a ^ b;
    prints("\nActual: ");
    *ConsoleWriteSInt = xorTest(a, b);

    prints("\n--------------------\n");
    prints("\nxori\nExpected: ");
    *ConsoleWriteSInt = a ^ 10;
    prints("\nActual: ");
    *ConsoleWriteSInt = xorTest(a, 10);


    prints("\n--------------------\n");
    prints("\nor\nExpected: ");
    *ConsoleWriteSInt = a | b;
    prints("\nActual: ");
    *ConsoleWriteSInt = orTest(a, b);

    prints("\n--------------------\n");
    prints("\nori\nExpected: ");
    *ConsoleWriteSInt = a | 10;
    prints("\nActual: ");
    *ConsoleWriteSInt = orTest(a, 10);

    prints("\n--------------------\n");
    prints("\nand\nExpected: ");
    *ConsoleWriteSInt = a & b;
    prints("\nActual: ");
    *ConsoleWriteSInt = andTest(a, b);

    prints("\n--------------------\n");
    prints("\nandi\nExpected: ");
    *ConsoleWriteSInt = a & 10;
    prints("\nActual: ");
    *ConsoleWriteSInt = andTest(a, 10);

    prints("\n--------------------\n");
    prints("\nsll\nExpected: ");
    *ConsoleWriteSInt = a << b;
    prints("\nActual: "); 
    *ConsoleWriteSInt = sllTest(a, b);

    prints("\n--------------------\n");
    prints("\nsra\nExpected: ");
    *ConsoleWriteSInt = a >> b; // SRA is the proper right shift 
    prints("\nActual: "); 
    *ConsoleWriteSInt = sraTest(a, b);

    prints("\n--------------------\n");
    prints("\nsrl\nExpected: ");
    *ConsoleWriteSInt = 0; // This is just brute right shifting
    prints("\nActual: "); 
    *ConsoleWriteSInt = srlTest(a, b);



    prints("\n");
    return 0;
}