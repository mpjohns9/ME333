#include "position.h"
#include "current.h"
#include <stdio.h>

#define EINTMAX 1000

volatile float position_kp = 100.00;
volatile float position_ki = 0.00;
volatile float position_kd = 1000.00;

volatile float d_pos[5000];
volatile float a_pos[5000];
volatile int d_len = 0;

volatile float d_angle = 0;

void set_position_gains(float kp, float ki, float kd){
    position_kp = kp;
    position_ki = ki;
    position_kd = kd;
}

float get_position_ki(){
    return position_ki;
}

float get_position_kp(){
    return position_kp;
}

float get_position_kd(){
    return position_kd;
}

void Position_ISR_Startup(){
    T4CONbits.TCKPS = 3;
    PR4 = 49999;
    TMR4 = 0;
    T4CONbits.ON = 1;
    IPC4bits.T4IP = 4;
    IPC4bits.T4IS = 0;
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
}

void __ISR(_TIMER_4_VECTOR, IPL4SOFT) Timer4ISR(void){

    static int counter = 0;
    static float eint = 0;
    static float eprev = 0;

    switch(get_mode()){
        case TRACK:
        {
            char m[50];
            // int test = 888;
            // sprintf(m, "%d\r\n", test);
            // NU32_WriteUART3(m);

            WriteUART2("a");
            while(!get_encoder_flag()) {}
            set_encoder_flag(0);
            
            float pos = (get_encoder_count()*360.0)/(4*334.0);
            a_pos[counter] = pos;
            float err = d_pos[counter] - pos;
            eint = eint + err;

            if (eint > EINTMAX){
                eint = EINTMAX;
            }
            if (eint < -EINTMAX){
                eint = -EINTMAX;
            }

            float edot = err - eprev;
            eprev = err;
            float u = position_kp*err + position_ki*eint + position_kd*edot;

            set_desired_torque(u);

            counter++;
            if (counter == d_len){
                counter = 0;
                eint = 0;
                eprev = 0;
                set_desired_angle((get_encoder_count()*360.0)/(4*334.0));
                set_mode(HOLD);
            }

            break;

        }

        case HOLD:
        {
            WriteUART2("a");
            while(!get_encoder_flag()) {}
            set_encoder_flag(0);
            
            float pos = (get_encoder_count()*360.0)/(4*334.0);
            float err = d_angle - pos;

            eint = eint + err;

            if (eint > EINTMAX){
                eint = EINTMAX;
            }
            if (eint < -EINTMAX){
                eint = -EINTMAX;
            }

            float edot = err - eprev;
            eprev = err;
            float u = position_kp*err + position_ki*eint + position_kd*edot;
            // char m[50];
            // sprintf(m, "%f\r\n", u);
            // NU32_WriteUART3(m);

            set_desired_torque(u);

            break;
        }
    }
    IFS0bits.T4IF = 0;
}

void set_desired_position(int index, float pos){
    if (index < 2000){
        d_pos[index] = pos;
    }
}

void set_desired_len(int len){
    d_len = len;
}

void set_desired_angle(float a){
    d_angle = a;
}

void TRACK_print(){
    char m[50];
    sprintf(m, "%d\r\n", d_len);
    NU32_WriteUART3(m);
    for (int i=0; i<d_len; i++){
        sprintf(m, "%f %f\r\n", d_pos[i], a_pos[i]);
        NU32_WriteUART3(m);
    }
}