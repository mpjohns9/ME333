#ifndef CURRENT__H__
#define CURRENT__H__


#include <xc.h>
#include <sys/attribs.h>

#include "NU32.h"
#include "utilities.h"
#include "ina219.h"
#include "position.h"

void Current_ISR_Startup();
void set_current_gains(float, float);
float get_current_ki();
float get_current_kp();
void ITEST_print();
void set_desired_torque(float);

#endif