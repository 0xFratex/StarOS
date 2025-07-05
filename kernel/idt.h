// kernel/idt.h - Interface para a Tabela de Descritores de Interrupção (IDT)

#ifndef IDT_H
#define IDT_H

#include <kernel/types.h> // Usaremos tipos de tamanho fixo

// Estrutura de uma entrada na IDT
struct idt_entry {
    u16 base_lo;    // Endereço base (bits 0-15)
    u16 sel;        // Seletor de segmento do Kernel
    u8  always0;    // Deve ser sempre zero
    u8  flags;      // Flags de tipo e atributos
    u16 base_hi;    // Endereço base (bits 16-31)
} __attribute__((packed)); // Garante que o compilador não otimize o alinhamento

// Estrutura do ponteiro para a IDT (usado pela instrução lidt)
struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

// Declaração de um array de 256 entradas da IDT
extern struct idt_entry idt[256];

// Função para inicializar a IDT
void idt_init();

#endif// kernel/idt.h - Interface para a Tabela de Descritores de Interrupção (IDT)

#ifndef IDT_H
#define IDT_H

#include <kernel/types.h> // Usaremos tipos de tamanho fixo

// Estrutura de uma entrada na IDT
struct idt_entry {
    u16 base_lo;    // Endereço base (bits 0-15)
    u16 sel;        // Seletor de segmento do Kernel
    u8  always0;    // Deve ser sempre zero
    u8  flags;      // Flags de tipo e atributos
    u16 base_hi;    // Endereço base (bits 16-31)
} __attribute__((packed)); // Garante que o compilador não otimize o alinhamento

// Estrutura do ponteiro para a IDT (usado pela instrução lidt)
struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

// Declaração de um array de 256 entradas da IDT
extern struct idt_entry idt[256];

// Função para inicializar a IDT
void idt_init();

#endif