#include <stdio.h>
int main(void) {
        unsigned char i, j, k, ij, ik, jk;
        i = 60;
        j = 80;
        k = 200;

        ij = i+j;
        ik = i+k;
        jk = j+k;

        printf("%d\n", ij);
        printf("%d\n", ik);
        printf("%d\n", jk);
        return 0;
}