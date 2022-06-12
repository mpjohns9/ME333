#include "current.h"
#include <stdio.h>

#define EINTMAX 1000

volatile float curr_kp = 0.1;
volatile float curr_ki = 0.05;

volatile float d_curr[5000];
volatile float a_curr[5000];
volatile float d_torque = 0;

void set_desired_torque(float u) {
    d_torque = u;
}

void set_current_gains(float kp, float ki) {
    // char m[50];
    // sprintf(m, "%f %f\r\n", kp, ki);
    // NU32_WriteUART3(m);
    curr_kp = kp;
    curr_ki = ki;
    // sprintf(m, "%f %f\r\n", curr_kp, curr_ki);
    // NU32_WriteUART3(m);
}

float get_current_ki() {
    return curr_ki;
}

float get_current_kp() {
    return curr_kp;
}

void Current_ISR_Startup() {

    T3CONbits.TCKPS = 1;
    PR3 = 1999;
    TMR3 = 0;

    OC1CONbits.OCM = 0b110;
    OC1CONbits.OCTSEL = 1;
    OC1RS = 0;
    OC1R = 0;
    T3CONbits.ON = 1;
    OC1CONbits.ON = 1;

    TRISDbits.TRISD1 = 0;
    LATDbits.LATD1 = 0;

    T2CONbits.TCKPS = 2;
    PR2 = 3999;
    TMR2 = 0;
    T2CONbits.ON = 1;
    IPC2bits.T2IP = 6;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void __ISR(_TIMER_2_VECTOR, IPL6SRS) Timer2ISR(void) {

    // char m[50];
    // sprintf(m, "%f\r\n", get_current_kp());
    // NU32_WriteUART3(m);

    static int counter = 0;
    static float desired_curr = 200.0;
    static float eint = 0;

    // int test = 444;
    // sprintf(m, "%d\r\n", test);
    // NU32_WriteUART3(m);
    
    switch(get_mode()){

        case IDLE:
        {
            char m[50];
            OC1RS = 0;
            LATDbits.LATD1 = 0;
            break;
        }

        case PWM:
        {
            char m[50];
            int p = get_pwm();
            if (p > 0) {
                OC1RS = 1999*p/100;
                LATDbits.LATD1 = 1;
            }
            else {
                p = -p;
                OC1RS = 1999*p/100;
                LATDbits.LATD1 = 0;
            }
            break;
        }
        
        case ITEST:
        {
            if (counter == 25){
                desired_curr = -200.0;
            }
            if (counter == 50){
                desired_curr = 200.0;
            }
            if (counter == 75){
                desired_curr = -200.0;
            }

            float curr = INA219_read_current();
            float err = desired_curr - curr;
            eint = eint + err;
            
            if (eint > EINTMAX){
                eint = EINTMAX;
            }
            if (eint < -EINTMAX){
                eint = -EINTMAX;
            }

            float u = curr_kp*err + curr_ki*eint;

            if (u > 100.0){
                u = 100.0;
            }
            if (u < -100.0){
                u = -100.0;
            }
            if (u > 0){
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 1;
            }
            else {
                u = -u;
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 0;
            }

            d_curr[counter] = desired_curr;
            a_curr[counter] = curr;

            counter++;

            if (counter == 100){
                counter = 0;
                desired_curr = 200.0;
                eint = 0;
                set_mode(IDLE);
            }
            break;
        }

        case TRACK:
        {
            float curr = INA219_read_current();
            float err = d_torque - curr;
            eint = eint + err;

            if (eint > EINTMAX){
                eint = EINTMAX;
            }

            if (eint < -EINTMAX){
                eint = -EINTMAX;
            }
            float u = curr_kp*err + curr_ki*eint;

            if (u > 100.0){
                u = 100.0;
            }
            if (u < -100.0){
                u = -100.0;
            }
            if (u > 0){
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 1;
            }
            else {
                u = -u;
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 0;
            }
            break;
        }

        case HOLD:
        {
            float curr = INA219_read_current();
            float err = d_torque - curr;
            eint = eint + err;

            if (eint > EINTMAX){
                eint = EINTMAX;
            }

            if (eint < -EINTMAX){
                eint = -EINTMAX;
            }
            float u = curr_kp*err + curr_ki*eint;
            // char m[50];
            // sprintf(m, "%f\r\n", u);
            // NU32_WriteUART3(m);

            if (u > 100.0){
                u = 100.0;
            }
            if (u < -100.0){
                u = -100.0;
            }
            if (u > 0){
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 1;
            }
            else {
                u = -u;
                OC1RS = 1999*u/100;
                LATDbits.LATD1 = 0;
            }
            break;
        }

    }
    IFS0bits.T2IF = 0;
}

void ITEST_print(){
    char m[50];
    NU32_WriteUART3("100\r\n");
    for(int i=0; i<100; i++){
        sprintf(m, "%f %f\r\n", d_curr[i], a_curr[i]);
        NU32_WriteUART3(m);
    }
}