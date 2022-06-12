#ifndef UTIL__H__
#define UTIL__H__

enum mode_t {IDLE, PWM, ITEST, HOLD, TRACK};

volatile enum mode_t get_mode(void);

void set_mode(enum mode_t);

void set_pwm(int p);

int get_pwm();

#endif