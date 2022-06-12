#include "NU32.h"          // config bits, constants, funcs for startup and UART
#include "encoder.h"
#include "ina219.h"
#include "utilities.h"
#include "current.h"
#include "position.h"
#include <stdio.h>

#define BUF_SIZE 200

int main() 
{
  char buffer[BUF_SIZE];
  NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  NU32_LED1 = 1;  // turn off the LEDs
  NU32_LED2 = 1;    
  set_mode(IDLE);    
  __builtin_disable_interrupts();
  UART2_Startup();
  INA219_Startup();
  Current_ISR_Startup();
  Position_ISR_Startup();
  __builtin_enable_interrupts();

  while(1)
  {
    NU32_ReadUART3(buffer,BUF_SIZE); // we expect the next character to be a menu command
    NU32_LED2 = 1;                   // clear the error LED
    switch (buffer[0]) {
      // case 'a':
      // {
      //   float ma = INA219_read_current();
      //   char m[50];
      //   sprintf(m, "%f\r\n", ma);
      //   NU32_WriteUART3(m);
      //   break;
      // }
      case 'b':
      {
        float ma = INA219_read_current();
        char m[50];
        sprintf(m, "%d\r\n", (int) ma);
        NU32_WriteUART3(m);
        break;
      }
      case 'c':
      {
        // read encoder counts
        WriteUART2("a");
        while(!get_encoder_flag()) {}
        set_encoder_flag(0);
        char m[50];
        int p = get_encoder_count()*3;
        sprintf(m, "%d\r\n", p);
        NU32_WriteUART3(m);
        break;
      }
      case 'd':
      {
        // read encoder degrees
        WriteUART2("a");
        while(!get_encoder_flag()) {}
        set_encoder_flag(0);
        char m[50];
        int p = get_encoder_count();
        sprintf(m, "%d\r\n", (int) (p*360)/(4*334));
        NU32_WriteUART3(m);
        break;
      }
      case 'e':
      {
        // reset encoder
        WriteUART2("b");
        break;
      }
      case 'f':
      {
        // set the PWM

        char m[50];
        int p;

        NU32_ReadUART3(m, 50);
        sscanf(m, "%d", &p);

        set_pwm(p);
        set_mode(PWM);

        char n[50];
        sprintf(n, "%d\r\n", p);
        NU32_WriteUART3(n);
        break;
      }
      case 'g':
      {
        // set current PI gains

        float temp_kp = 0.0;
        float temp_ki = 0.0;

        char m[50];
        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &temp_kp);

        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &temp_ki);

        set_current_gains(temp_kp, temp_ki);
        break;
      }
      case 'h':
      {
        char m[50];
        sprintf(m, "%f\r\n", get_current_kp());
        NU32_WriteUART3(m);

        sprintf(m, "%f\r\n", get_current_ki());
        NU32_WriteUART3(m);
        break;
      }
      case 'i':
      {
        // set posiiton PID gains

        float temp_kp = 0.0;
        float temp_ki = 0.0;
        float temp_kd = 0.0;
        
        char m[50];
        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &temp_kp);

        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &temp_ki);

        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &temp_kd);

        set_position_gains(temp_kp, temp_ki, temp_kd);
        break;
      }
      case 'j':
      {
        char m[50];
        sprintf(m, "%f\r\n", get_position_kp());
        NU32_WriteUART3(m);

        sprintf(m, "%f\r\n", get_position_ki());
        NU32_WriteUART3(m);

        sprintf(m, "%f\r\n", get_position_kd());
        NU32_WriteUART3(m);
        break;
      }
      case 'k':
      {
        // ITEST
        set_mode(ITEST);
        while(get_mode() == ITEST) {}
        ITEST_print();
        break;
      }
      case 'l':
      {
        // go to angle
        char m[50];
        float a;

        NU32_ReadUART3(m, 50);
        sscanf(m, "%f", &a);

        set_desired_angle(a);
        set_mode(HOLD);

        char n[50];
        sprintf(n, "%f\r\n", a);
        NU32_WriteUART3(n);
        break;
      }
      case 'm':
      {
        // load a trajectory
        int len = 0;
        char m[50];
        NU32_ReadUART3(m, 50);
        sscanf(m, "%d", &len);
        set_desired_len(len);
        float pos = 0;
        for (int i=0; i<len; i++){
          NU32_ReadUART3(m, 50);
          sscanf(m, "%f", &pos);
          set_desired_position(i, pos);
        }
        break;
      }
      case 'n':
      {
        // load a trajectory
        int len = 0;
        char m[50];
        NU32_ReadUART3(m, 50);
        sscanf(m, "%d", &len);
        set_desired_len(len);
        float pos = 0;
        for (int i=0; i<len; i++){
          NU32_ReadUART3(m, 50);
          sscanf(m, "%f", &pos);
          set_desired_position(i, pos);
        }
        break;
      }
      case 'o':
      {
        // track a trajectory
        set_mode(TRACK);
        while(get_mode() == TRACK) {}
        TRACK_print();
        break;
      }
      case 'p':
      {
        // unpower the motor
        set_mode(IDLE);
        break;
      }
      case 'q':
      {
        // handle q for quit. Later you may want to return to IDLE mode here. 
        set_mode(IDLE);
        break;
      }
      case 'r':
      {
        char m[50];
        sprintf(m, "%d\r\n", get_mode());
        NU32_WriteUART3(m);
        break;
      }
      default:
      {
        NU32_LED2 = 0;  // turn on LED2 to indicate an error
        break;
      }
    }
  }
  return 0;
}
