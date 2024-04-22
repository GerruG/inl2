#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN     PD3 // Define the pin where the LED is connected
#define LED_DDR     DDRD // Define the data direction register for the LED
#define LED_PORT    PORTD // Define the port for the LED

// Function to initialize the LED pin
void init_led() {
    LED_DDR |= (1 << LED_PIN); // Set LED pin as an output
}

// Function to initialize TIMER1
// Function to initialize TIMER1
void init_timer1() {
    TCCR1A = 0; // Set timer to normal mode
    TCCR1B = (1 << WGM12) | (1 << CS12); // Configure for CTC mode and prescaler of 256
    OCR1A = 12499; // Set output compare register for 200ms intervals (assuming 16MHz clock)
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
