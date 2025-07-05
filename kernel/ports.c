// kernel/ports.c - Implementação das funções de porta

#include <kernel/ports.h>

unsigned char inb(unsigned short port) {
    unsigned char result;
    // "in al, dx" - Lê o byte da porta em dx para o registrador al
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void outb(unsigned short port, unsigned char data) {
    // "out dx, al" - Escreve o byte do registrador al para a porta em dx
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}