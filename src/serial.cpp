#include "serial.h"
#include <avr/io.h>
#include <stdbool.h>

#define BAUD 9600
#define MYUBRR ((F_CPU / 16 / BAUD) - 1)
#define UART_8N1 ((1 << UCSZ01) | (1 << UCSZ00))
#define UART_ENABLE_RX_TX ((1 << RXEN0) | (1 << TXEN0))
#define UART_BUFFER_READY (1 << UDRE0)
#define UART_DATA_RECEIVED (1 << RXC0)

void init_uart() {
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    UCSR0B = UART_ENABLE_RX_TX;
    UCSR0C = UART_8N1;
}

void uart_putchar(char c) {
    while (!(UCSR0A & UART_BUFFER_READY));
    UDR0 = c;
}

void uart_putstr(const char *s) {
    while (*s) {
        uart_putchar(*s++);
    }
    uart_putchar('\n');
}

void uart_putint(int val) {
    char numStr[12];
    sprintf(numStr, "%d", val);
    uart_putstr(numStr);
}

char uart_getchar() {
    while (!(UCSR0A & UART_DATA_RECEIVED));
    return UDR0;
}

void uart_rec_str(char *str, unsigned int max_len) {
    unsigned int i = 0;
    char c;
    while (i < max_len - 1) {
        c = uart_getchar();
        if (c == '\n' || c == '\r') {
            break;
        }
        str[i++] = c;
    }
    str[i] = '\0';
}
