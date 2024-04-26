#include "serial.h"
#include <avr/io.h>
#include <stdbool.h>

#define BAUD 9600  // Define baud rate
#define MYUBRR ((F_CPU / 16 / BAUD) - 1)  // Calculate UBRR value
#define UART_8N1 ((1 << UCSZ01) | (1 << UCSZ00))  // UART frame format: 8 data, 1 stop, no parity
#define UART_ENABLE_RX_TX ((1 << RXEN0) | (1 << TXEN0))  // Enable UART receiver and transmitter
#define UART_BUFFER_READY (1 << UDRE0)  // UART buffer ready flag
#define UART_DATA_RECEIVED (1 << RXC0)  // UART data received flag

// Initialize UART with specified baud rate and settings
void init_uart() {
    UBRR0H = (unsigned char)(MYUBRR >> 8);  // Set baud rate high byte
    UBRR0L = (unsigned char)MYUBRR;  // Set baud rate low byte
    UCSR0B = UART_ENABLE_RX_TX;  // Enable UART receiver and transmitter
    UCSR0C = UART_8N1;  // Set UART frame format: 8 data, 1 stop, no parity
}

// Transmit a single character over UART
void uart_putchar(char c) {
    while (!(UCSR0A & UART_BUFFER_READY));  // Wait for the buffer to be ready for new data
    UDR0 = c;  // Put the data into the buffer, sends the data
}

// Transmit a null-terminated string over UART followed by a newline character
void uart_putstr(const char *s) {
    while (*s) {  // Loop until end of string
        uart_putchar(*s++);  // Send each character
    }
    uart_putchar('\n');  // Send newline character after the string
}

// Transmit an integer value over UART as a string
void uart_putint(int val) {
    char numStr[12];  // Buffer to hold string representation of integer
    sprintf(numStr, "%d", val);  // Convert integer to string
    uart_putstr(numStr);  // Send the string over UART
}

// Receive a single character from UART
char uart_getchar() {
    while (!(UCSR0A & UART_DATA_RECEIVED));  // Wait for data to be received
    return UDR0;  // Return received data
}

// Receive a string from UART until a newline character or the maximum length is reached
void uart_rec_str(char *str, unsigned int max_len) {
    unsigned int i = 0;  // Initialize index for the string
    char c;  // Variable to hold received character
    while (i < max_len - 1) {  // Loop until maximum length or newline character
        c = uart_getchar();  // Receive a character
        if (c == '\n' || c == '\r') {
            break;
        }
        str[i++] = c;  // Store received character in the string
    }
    str[i] = '\0';  // Null-terminate the string
}
