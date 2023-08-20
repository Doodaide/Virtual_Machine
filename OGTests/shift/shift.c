
int volatile *const ConsoleWriteHex = (int *)0x0808;
inline int scan_char() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

void prints(char *str){
    while(*str) *((char *)0x0800) = *(str++);
}

inline int lshift(int num, int cnt){
    int result;
    asm volatile("sll %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(cnt), [num]"r"(num));
    return result;
}

inline int rshift(int num, int cnt){
    int result;
    asm volatile("srl %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(cnt), [num]"r"(num));
    return result;
}


int main(){
    int num = scan_char();
    int count = scan_char();

    prints("0x");
    *ConsoleWriteHex = lshift(num, count);
    prints("\n");

    prints("0x");
    *ConsoleWriteHex = rshift(num, count);
    prints("\n");
    
    return 0;
}

