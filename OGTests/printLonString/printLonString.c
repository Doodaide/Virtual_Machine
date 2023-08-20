void prints(char * str)
{
    while(*str)
    {
        *((char*) 0x0800) = *(str++);
    }
}

int main()
{
    prints("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
    return 0;
}
