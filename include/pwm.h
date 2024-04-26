#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void init_pwm();
void set_pwm(uint8_t duty_cycle);

#endif
