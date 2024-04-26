#include "timer.h"

#define TIMER1_CTC_PRESCALER_1024 ((1 << WGM12) | (1 << CS12) | (1 << CS10))
#define TIMER1_CTC_MATCH 15624
#define TIMER1_COMPARE_MATCH_A (1 << OCIE1A)
#define PWM_TOGGLE_CONNECTION (1 << COM2B1)

void init_timer1() {
    TCCR1A = 0;
    TCCR1B = TIMER1_CTC_PRESCALER_1024;
    OCR1A = TIMER1_CTC_MATCH;
    TIMSK1 = TIMER1_COMPARE_MATCH_A;
}

void change_frequency(uint16_t ms) {
    uint32_t compare = (F_CPU / 1024) * ms / 1000 - 1;
    OCR1A = (uint16_t)compare;
}

ISR(TIMER1_COMPA_vect) {
    TCCR2A ^= PWM_TOGGLE_CONNECTION;
}
