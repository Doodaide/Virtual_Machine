
void prints(char *str){
    while(*str) *((char *)0x0800) = *(str++);
}

int main() {
    prints("Hello, world!\n");
    return 0;
}