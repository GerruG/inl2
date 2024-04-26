#include "timer.h"

#define TIMER1_CTC_PRESCALER_1024 ((1 << WGM12) | (1 << CS12) | (1 << CS10))  // Prescaler for Timer1 with CTC mode
#define TIMER1_CTC_MATCH 15624  // Value for the match register to achieve desired frequency
#define TIMER1_COMPARE_MATCH_A (1 << OCIE1A)  // Enable interrupt on compare match A
#define PWM_TOGGLE_CONNECTION (1 << COM2B1)  // Toggle PWM connection

void init_timer1() {
    TCCR1A = 0;  // Clear Timer1 control register A
    TCCR1B = TIMER1_CTC_PRESCALER_1024;  // Set prescaler and waveform generation mode for Timer1
    OCR1A = TIMER1_CTC_MATCH;  // Set compare match value for Timer1
    TIMSK1 = TIMER1_COMPARE_MATCH_A;  // Enable compare match A interrupt for Timer1
}

void change_frequency(uint16_t ms) {
    // Calculate new compare match value based on desired frequency
    uint32_t compare = (F_CPU / 1024) * ms / 1000 - 1;
    OCR1A = (uint16_t)compare;  // Update compare match register for Timer1
}

ISR(TIMER1_COMPA_vect) {
    TCCR2A ^= PWM_TOGGLE_CONNECTION;  // Toggle PWM connection on compare match A interrupt for Timer1
}
