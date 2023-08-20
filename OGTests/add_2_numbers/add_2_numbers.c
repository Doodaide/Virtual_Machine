inline int scan_char() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

int volatile *const ConsoleReadSInt = (int *)0x0816;
int volatile *const ConsoleWriteSInt = (int *)0x0804;
int main() {

    int a = scan_char();
    int b = scan_char();

    *ConsoleWriteSInt = a + b;

    return 0;
}
