void prints(char *str){
    while(*str) *((char *)0x0800) = *(str++);
}

inline int scan_char() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

int volatile *const ConsoleReadSInt = (int *)0x0816;
int volatile *const ConsoleWriteSInt = (int *)0x0804;
int main() {
	prints("Input first number: ");
    int a = scan_char();
	prints("\n");
	prints("Input second number: ");
    int b = scan_char();
	prints("\n");
	int i = 0;
	while (i < b)
	{
		a = a + a;
		i = i + 1;
	}
    *ConsoleWriteSInt = (a);

    return 0;
}
