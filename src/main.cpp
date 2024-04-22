#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN     PD3 // Define the pin where the LED is connected
#define LED_DDR     DDRD // Define the data direction register for the LED
#define LED_PORT    PORTD // Define the port for the LED

void init_led() {
    LED_DDR |= (1 << LED_PIN); // Set LED pin as an output
}
void init_timer1() {
    TCCR1A = 0; // Set timer to normal mode
    TCCR1B = (1 << WGM12) | (1 << CS12); // Configure for CTC mode and prescaler of 256
    OCR1A = 12499; // Set output compare register for 200 ms intervals
    // Calculation: OCR1A = (Clock Frequency / (Prescaler * Target Frequency)) - 1
    //              = (16 MHz / (256 * 5 Hz)) - 1
    //              = 12499
    // This setting results in a timer frequency of 5 Hz, or a period of 200 ms.
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt A
}

// Timer1 Compare Match A interrupt service routine
ISR(TIMER1_COMPA_vect) {
    LED_PORT ^= (1 << LED_PIN); // Toggle the LED state
}

// Main function
int main(void) {
    init_led(); // Initialize the LED
    init_timer1(); // Initialize TIMER1
    sei(); // Enable global interrupts

    while(1) {}
}
