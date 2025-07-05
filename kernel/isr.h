// kernel/isr.h - Interface para os Manipuladores de Serviço de Interrupção (ISRs)

#ifndef ISR_H
#define ISR_H

#include <kernel/types.h>

// Estrutura que contém os registradores salvos na pilha pela rotina de stub
typedef struct {
    u32 ds;                                      // Segmento de dados
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;   // Registradores salvos por pusha
    u32 int_no, err_code;                        // Número da interrupção e código de erro
    u32 eip, cs, eflags, useresp, ss;            // Salvos pelo processador automaticamente
} registers_t;

// Um manipulador de interrupção é um ponteiro para uma função que recebe os registradores
typedef void (*isr_t)(registers_t*);

// Função para registrar um manipulador para uma interrupção específica
void register_interrupt_handler(u8 n, isr_t handler);

#endif