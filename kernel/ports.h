// kernel/ports.h - Funções para interagir com as portas de I/O

#ifndef PORTS_H
#define PORTS_H

// Lê um byte de uma porta
unsigned char inb(unsigned short port);

// Escreve um byte em uma porta
void outb(unsigned short port, unsigned char data);

#endif