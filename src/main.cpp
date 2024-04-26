#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"
#include "pwm.h"
#include "timer.h"
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL  // Define CPU frequency if not defined
#endif

// Handle received command
void handle_command(const char *cmd) {
    int power, frequency;  // Variables to store power and frequency values
    uart_putstr("Received command: ");  // Print received command
    uart_putstr(cmd);

    // Parse command and extract power and frequency values
    if (sscanf(cmd, "ledpowerfreq %d %d", &power, &frequency) == 2) {
        // Check if power and frequency values are within valid range
        if (power < 0 || power > 255 || frequency < 200 || frequency > 5000) {
            uart_putstr("Invalid parameters. Please use the format 'ledpowerfreq <power 0-255> <frequency 200-5000 ms>'.\n");
        } else {
            // Set PWM power and timer frequency
            set_pwm((uint8_t)power);  // Set PWM power
            change_frequency((uint16_t)frequency);  // Set timer frequency
            // Print confirmation message
            uart_putstr("LED power set to ");
            uart_putint(power);
            uart_putstr(" and blinking frequency set to ");
            uart_putint(frequency);
            uart_putstr(" ms\n");
        }
    } else {
        uart_putstr("Invalid command format. Use 'ledpowerfreq <power> <frequency>'.\n");  // Print error message for invalid command format
    }
}

int main(void) {
    char cmd[50];  // Buffer to store received command
    init_uart();  // Initialize UART communication
    init_pwm();  // Initialize PWM
    init_timer1();  // Initialize Timer1
    sei();  // Enable global interrupts

    while (1) {
        uart_putstr("Enter command (ledpowerfreq <power> <frequency>): ");  // Prompt user for command
        uart_rec_str(cmd, sizeof(cmd));  // Receive command from user
        handle_command(cmd);  // Handle received command
    }

    return 0;  // Return success
}

