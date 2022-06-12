/*
 * ascii.c
 *
 * This program generates the ASCII table for decimal values
 * from 33 to 127. The output is two columns, with the ASCII
 * characters on the right side and corresponding decimals on 
 * the left.  
 *
 * This program was written to answer question 34 from ME333 HW1.
 *
 */

#include <stdio.h>

int main(void){

    // Loop through values 33 to 127 and print the current iteration 
    // number (decimal) alongside the corresponding ASCII char
    unsigned char i;
    for (i=33; i<=127; i++){

        if (i<100) {
            printf("  %d   | %c  ", i, i);
        }

        else if (i==127) {
            printf("  %d  | DEL  ", i);
        }
            
        else {
            printf("  %d  | %c  ", i, i);
        } 

        if (i%4 == 0) {
            printf("\n");
            // printf("  DEC  | ASCII \n");
            // printf(" -------------- \n");
        }
    }
    printf("\n");
    return 0;
}