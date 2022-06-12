/*
 * shift.c
 *
 * This program takes an input string and number and shifts
 * each character of the string over by the number given, 
 * according to the ASCII table.  
 *
 * This program was written to answer Question 9 of ME333 Quiz 1.
 *
 */

#include <stdio.h>
#include <string.h>

static char s[100];
static int shift = 0;

int main(void) {

    // Take user input and assign to variables
    printf("Please enter a string followed by a number with a space in between.\nThe number should be between 1 and 8 (inclusive):\n");
    scanf("%s%d", s, &shift);

    // If user inputs number out of range, ask for another number
    while (shift < 1 || shift > 8) {
        printf("Your number is out of range. Please enter a number between 1 and 8:\n");
        scanf("%d", &shift);
    }

    // Print out adjusted string
    printf("Your adjusted string is:\n");
    int i;
    for (i=0;i<strlen(s);i++) {
        printf("%c", s[i]+shift);
    }
    printf("\n");

    return 0;
}

