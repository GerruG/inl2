#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

void init_timer1();
void change_frequency(uint16_t ms);

#endif
