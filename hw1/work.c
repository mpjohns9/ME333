#include <stdio.h>
int main(void) {
    unsigned char a=0x0D, b=0x03, c;

    c = a|b;

    printf("%x\n", c);
}