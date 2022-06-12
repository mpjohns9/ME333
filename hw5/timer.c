#include "NU32.h" 
#include <stdio.h>

#define DELAYTIME 40000000

void delay(void);

static volatile double start = 0, end = 0;
static volatile unsigned int state = 1;

void __ISR(_EXTERNAL_2_VECTOR, IPL6SRS) Ext2ISR(void) {
    // unsigned int current_time = _CP0_GET_COUNT();
    // while (_CP0_GET_COUNT() < (current_time + 300000)) {;}
    // NU32_WriteUART3("CLEARED");
    // if (!NU32_USER) {
    //     NU32_WriteUART3("MADE IT");
        char m[128] = {};

        if (state){
            
            start = _CP0_GET_COUNT();       // record time ISR begins
            sprintf(m, "START TIME: %f\n", start);

            NU32_WriteUART3(m);
            IFS0bits.INT2IF = 0;                 // clear the interrupt flag

            state = 0;
        }

        else {
            end = _CP0_GET_COUNT();        // record time ISR ends
            sprintf(m, "END TIME: %f\n", end);

            NU32_WriteUART3(m);
            IFS0bits.INT2IF = 0;                 // clear the interrupt flag

            state = 1;
        }
    // }
    // IFS0bits.INT2IF = 0;

    
}

int main(void) {
    double dt = 0;
    double start_copy, end_copy;      // local copies of globals Entered, Exited
    char msg[128] = {};

    NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    __builtin_disable_interrupts();   // disable interrupts at CPU
    INTCONbits.INT2EP = 1;            // INT2 trigger on rising edge
    IPC2bits.INT2IP = 0;              // clear priority and subp bits for INT2
    IPC2bits.INT2IS = 0;              // clear priority and subp bits for INT2
    IPC2bits.INT2IP = 6;              // set INT2 to priority 6 subpriority 0
    IPC2bits.INT2IS = 0;              // set INT2 to priority 6 subpriority 0
    IFS0bits.INT2IF = 0;              // clear INT2 flag status
    IEC0bits.INT2IE = 1;               // enable INT2 interrupt
    __builtin_enable_interrupts();    // enable interrupts

    _CP0_SET_COUNT(0);

    NU32_WriteUART3("Press the USER button to start the timer.\n");
    while(NU32_USER) {;} 
    while(IFS0bits.INT2IF) {;}
    delay();


    NU32_WriteUART3("Press the USER button again to stop the timer.\n");
    while(NU32_USER) {;} 
    while(IFS0bits.INT2IF) {;}
    delay();

    __builtin_disable_interrupts(); // good practice before using vars shared w/ISR
    start_copy = start;               // copy the shared variables to local copies ...
    end_copy = end;                // ... so the time interrupts are off is short
    // sprintf(msg,"START: %f\n",start_copy);
    NU32_WriteUART3(msg);

    // sprintf(msg,"END: %f\n",end_copy);
    NU32_WriteUART3(msg);

    dt = ((end_copy-start_copy)*25)/1000000000;
    
    __builtin_enable_interrupts();  // turn interrupts back on quickly!
    sprintf(msg,"%.3f seconds elapsed.\n",dt);
    NU32_WriteUART3(msg);           // send times to the host
    return 0;
}

void delay() {
  unsigned int time = _CP0_GET_COUNT();
  while(_CP0_GET_COUNT() < (time + DELAYTIME)) {
      ;
  }
}