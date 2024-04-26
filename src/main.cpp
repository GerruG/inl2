#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial.h"
#include "pwm.h"
#include "timer.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

void handle_command(const char *cmd);

int main(void) {
    char cmd[50];
    init_uart();
    init_pwm();
    init_timer1();
    sei();

    while (1) {
        uart_putstr("Enter command (ledpowerfreq <power> <frequency>): ");
        uart_rec_str(cmd, sizeof(cmd));
        handle_command(cmd);
    }

    return 0;
}

void handle_command(const char *cmd) {
    int power, frequency;
    uart_putstr("Received command: ");
    uart_putstr(cmd);

    if (sscanf(cmd, "ledpowerfreq %d %d", &power, &frequency) == 2) {
        if (power < 0 || power > 255 || frequency < 200 || frequency > 5000) {
            uart_putstr("Invalid parameters. Please use the format 'ledpowerfreq <power 0-255> <frequency 200-5000 ms>'.\n");
        } else {
            set_pwm((uint8_t)power);
            change_frequency((uint16_t)frequency);
            uart_putstr("LED power set to ");
            uart_putint(power);
            uart_putstr(" and blinking frequency set to ");
            uart_putint(frequency);
            uart_putstr(" ms\n");
        }
    } else {
        uart_putstr("Invalid command format. Use 'ledpowerfreq <power> <frequency>'.\n");
    }
}
