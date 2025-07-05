// kernel/kernel.h - Declarações públicas do kernel

#ifndef KERNEL_H
#define KERNEL_H

// Declaração das funções de impressão que outros arquivos podem querer usar.
// A implementação (o código delas) continua em kernel.c.

void kernel_print(const char* str);
void kernel_print_char(char c);
void kernel_clear_screen();

// Helper functions for printing numbers
void kernel_print_hex(u32 n);
void kernel_print_dec(u32 n);

#endif
