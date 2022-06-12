#include <stdio.h>

int main() {
    int VA = 0x80000020;
    int PA;

    PA = VA & 0x1FFFFFFF;

    printf("%x", PA);
    return 0;
}