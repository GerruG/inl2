#include "pwm.h"

#define PWM_DDR DDRD
#define PWM_PIN PD3
#define PWM_TCCR2A_CONFIG ((1 << COM2B1) | (1 << WGM21) | (1 << WGM20))
#define PWM_PRESCALER_8 (1 << CS21)

// Initialize PWM on the specified pin
void init_pwm() {
    PWM_DDR |= (1 << PWM_PIN);  // Set PWM pin as output
    TCCR2A = PWM_TCCR2A_CONFIG;  // Configure Timer/Counter Control Register A for PWM
    TCCR2B = PWM_PRESCALER_8;  // Set prescaler for Timer/Counter Register 2
}

// Set PWM duty cycle (0-255) on the configured pin
void set_pwm(uint8_t duty_cycle) {
    OCR2B = duty_cycle;  // Set output compare register for PWM duty cycle
}
