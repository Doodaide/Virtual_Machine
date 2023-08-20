inline int scan_char() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}


int volatile *const ConsoleWriteSInt = (int *)0x0804;
int main() {
    int count = 0;
    int sum = 0;
    int num;

    while(count++ < 5){
        num = scan_char();
        if(num == 0) break;

        sum += num;
    }

    *ConsoleWriteSInt = sum;

    return 0;
}