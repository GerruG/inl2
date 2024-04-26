#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>  // for dtostrf
#include <stdio.h>   // for sprintf

#ifndef F_CPU
#define F_CPU 16000000UL // Define CPU frequency for delay calculations only if not already defined
#endif

#define BAUD 9600 // Define baud rate for serial communication
#define MYUBRR F_CPU/16/BAUD-1 // Calculate UBRR setting

#define LED_PIN     PD3
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define TIMER_INTERVAL_MS 10   // Interval for the timer interrupt in milliseconds

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_Print(const char* str) {
    while (*str) {
        USART_Transmit(*str++);
    }
}

void init_adc() {
    ADMUX = (1 << REFS0); // AVCC with external capacitor at AREF pin
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_adc(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void init_led() {
    LED_DDR |= (1 << LED_PIN);
}

void init_timer() {
    TCCR1A = 0; // Clear Timer on Compare Match mode (CTC)
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC mode, prescaler 64
    OCR1A = (F_CPU / 64 / 1000) * TIMER_INTERVAL_MS - 1; // Adjust OCR1A for accurate timing
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
}

volatile float voltage = 0.0; // Global variable for voltage

ISR(TIMER1_COMPA_vect) {
    static uint16_t toggle_count = 0;
    static uint16_t print_count = 0;
    uint16_t adc_result = read_adc(0);
    voltage = adc_result * 5.0 / 1023.0;

    toggle_count++;
    print_count++;

    if (voltage >= 5.0) {
        if (toggle_count >= 10) {
            LED_PORT ^= (1 << LED_PIN);
            toggle_count = 0;
        }
    } else if (voltage < 0.1) {
        LED_PORT |= (1 << LED_PIN);
    } else {
        uint16_t max_toggle = 10 * (5.0 - voltage);
        if (toggle_count >= max_toggle) {
            LED_PORT ^= (1 << LED_PIN);
            toggle_count = 0;
        }
    }

    if (print_count >= 100) {
        char buffer[60]; // Ensure buffer is large enough
        char voltage_str[10];
        dtostrf(voltage, 6, 2, voltage_str);
        sprintf(buffer, "ADC Value: %u, Voltage: %sV\n", adc_result, voltage_str);
        USART_Print(buffer);
        print_count = 0;
    }
}


int main(void) {
    USART_Init(MYUBRR);
    init_adc();
    init_led();
    init_timer();
    sei(); // Enable global interrupts

    while (1) {
        // All tasks are interrupt driven. The main loop does nothing.
    }
}
