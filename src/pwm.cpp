#include "pwm.h"

#define PWM_DDR DDRD
#define PWM_PIN PD3
#define PWM_TCCR2A_CONFIG ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20))
#define PWM_PRESCALER_8 (1 << CS21)

void init_pwm() {
    PWM_DDR |= (1 << PWM_PIN);
    TCCR2A = PWM_TCCR2A_CONFIG;
    TCCR2B = PWM_PRESCALER_8;
}

void set_pwm(uint8_t duty_cycle) {
    OCR2B = duty_cycle;
}
