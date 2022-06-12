/*
 * bubble.c
 *
 * This program sorts a string of text from the user in ascending
 * ASCII character order. The sort algorithm used is the bubble sort. 
 *
 * This program was written to answer question 35 from ME333 HW1.
 *
 */

#include <stdio.h>
#include <string.h>
#define MAXLENGTH 100 // max length of string input

// Helper function prototypes

void getString(char *str);
void printResult(char *str);
int greaterThan(char ch1, char ch2);
void swap(char *str, int index1, int index2);


int main(void) {
    int len; // length of the entered string
    char str[MAXLENGTH]; // array of length MAXLENGTH

    // call the getString function to get user input
    getString(str);

    len = strlen(str); // get length of the string, from string.h
    printf("The length of the string is %d.\n", len);

    // Loop through input string for iterations equal to length of 
    // given string. In each iteration, loop through string and swap
    // values that are out of order.
    while (len > 0) {
        int i;

        // Inner loop to swap characters that are not yet sorted
        for (i=0; i<len-1; i++) {

            // Check to see if next char is greater than curr
            // If so, call swap
            if (greaterThan(str[i], str[i+1])) {
                swap(str, i, i+1);
            }
        }

        // Decrement length counter 
        len -= 1;

        // Print sorted string
        printResult(str);
    }

    return(0);
}


/* getString
 *
 * This function assigns the input string to the str array.
 * 
 */
void getString(char *str) {
    printf("Enter string to be sorted using _ for spaces (up to %d characters): ", MAXLENGTH);
    scanf("%s", str);
}

/* printResult
 *
 * This function prints the str array.
 * 
 */
void printResult(char *str) {
    printf("%s\n", str);
}

/* getString
 *
 * This function takes two characters and checks whether or not the first
 * is greater than the second. Returns True if so, otherwise False.
 * 
 */
int greaterThan(char ch1, char ch2) {
    return(ch1 > ch2);
}

/* swap
 *
 * This function takes the str array, the index of the current char, and 
 * the index of the next char in the array. The current char is swapped
 * with the next char, modifying the str array in place.
 * 
 */
void swap(char *str, int index1, int index2) {
    char c1, c2;

    c1 = str[index1];
    c2 = str[index2];

    str[index1] = c2;
    str[index2] = c1;

}