#include "utilities.h"
#include <xc.h>

volatile enum mode_t mode;

int pwm = 0;

volatile enum mode_t get_mode(void){
    return mode;
}

void set_mode(enum mode_t m){
    mode = m;
}

void set_pwm(int p){
    pwm = p;
}

int get_pwm(){
    return pwm;
}