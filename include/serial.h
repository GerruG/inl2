#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>

void init_uart();
void uart_putchar(char c);
void uart_putstr(const char *s);
void uart_putint(int val);
char uart_getchar();
void uart_rec_str(char *str, unsigned int max_len);

#endif
