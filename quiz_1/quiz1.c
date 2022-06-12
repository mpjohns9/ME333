#include <stdio.h>
#include <string.h>

typedef struct {
        int id;
        char brand[50];
        float miles[10];
} car;

car calc_miles(car *cp);

int main(void) {
    
    car *cp;
    car c;
    c.id = 1;
    c.brand[0] = 0;

    int i;
    for (i=0;i<10;i++){
        c.miles[i] = 1;
        // printf("%f ", c.miles[i]);
    }

    cp = &c;
    c = calc_miles(cp);

    // printf("%d", c.miles);

    for (i=0;i<10;i++){
        printf("%f ", c.miles[i]);
    }

    // printf("Please enter the name of a car brand (max 50 characters).");
    // scanf("%s", c.brand);

    // int j;
    // for (j=0;j<strlen(c.brand);j++){
    //     printf("%c", c.brand[j]);
    // }

    // // printf("%d", *c.brand);
    return 0;
}

car calc_miles(car *cp) {
    
    int i;
    for (i=1;i<10;i++) {
        cp->miles[i] = cp->miles[i-1]*2;
    }
    return *cp;
}