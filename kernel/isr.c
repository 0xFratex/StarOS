// kernel/isr.c - Manipulador de interrupções genérico em C

#include <kernel/isr.h>
#include <kernel/ports.h>
#include <kernel/kernel.h> // Para usar kernel_print

// Array de ponteiros de função para os manipuladores.
// static significa que esta variável só é visível dentro deste arquivo.
static isr_t interrupt_handlers[256];

// Esta é a função que o nosso código Assembly (isr_common_stub) chama.
// A struct 'regs' é colocada na pilha pelo Assembly e passada como argumento aqui.
void isr_handler(registers_t regs) {
    
    // Verifica se existe uma função registrada no nosso array para essa interrupção.
    if (interrupt_handlers[regs.int_no] != 0) {
        
        // Se existe, pega o ponteiro da função.
        isr_t handler = interrupt_handlers[regs.int_no];
        
        // **AQUI ESTÁ A CORREÇÃO DEFINITIVA**
        // Chama a função (ex: keyboard_callback), passando para ela o
        // endereço de memória da struct 'regs'.
        handler(&regs);
        
    } else {
        // Se não houver um manipulador registrado, avisa no console.
        kernel_print("Unhandled interrupt: ");
        // Convert int_no to string and print it
        char str_int_no[4]; // Max 3 digits for 0-255, plus null terminator
        int num = regs.int_no;
        int i = 0;
        if (num == 0) {
            str_int_no[i++] = '0';
        } else {
            char temp[4];
            int temp_i = 0;
            while (num > 0) {
                temp[temp_i++] = (num % 10) + '0';
                num /= 10;
            }
            while (temp_i > 0) {
                str_int_no[i++] = temp[--temp_i];
            }
        }
        str_int_no[i] = '\0';
        kernel_print(str_int_no);
        kernel_print("\n");
    }
}

// Função para registrar um manipulador (callback) para uma interrupção específica.
void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
