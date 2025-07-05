// kernel/isr.c - Manipulador de interrupções genérico em C

#include <kernel/isr.h>
#include <kernel/ports.h>
#include <kernel/kernel.h> // Para usar kernel_print, kernel_print_hex, kernel_print_dec
#include <kernel/types.h> // Para u32

// Define this to enable detailed register printing for all interrupts
// #define DETAILED_INTERRUPT_DEBUG

// Array de ponteiros de função para os manipuladores.
// static significa que esta variável só é visível dentro deste arquivo.
static isr_t interrupt_handlers[256];

// Esta é a função que o nosso código Assembly (isr_common_stub) chama.
// A struct 'regs' é colocada na pilha pelo Assembly e passada como argumento aqui.
void isr_handler(registers_t regs) {
    kernel_print("Interrupt received: ");
    kernel_print_dec(regs.int_no);
    kernel_print(", Error code: ");
    kernel_print_hex(regs.err_code);
    kernel_print("\n");

#ifdef DETAILED_INTERRUPT_DEBUG
    kernel_print("  EAX: "); kernel_print_hex(regs.eax); kernel_print(" EBX: "); kernel_print_hex(regs.ebx);
    kernel_print(" ECX: "); kernel_print_hex(regs.ecx); kernel_print(" EDX: "); kernel_print_hex(regs.edx);
    kernel_print("\n");
    kernel_print("  ESI: "); kernel_print_hex(regs.esi); kernel_print(" EDI: "); kernel_print_hex(regs.edi);
    kernel_print(" EBP: "); kernel_print_hex(regs.ebp); kernel_print(" ESP: "); kernel_print_hex(regs.esp);
    kernel_print("\n");
    kernel_print("  EIP: "); kernel_print_hex(regs.eip); kernel_print(" CS: "); kernel_print_hex(regs.cs);
    kernel_print(" DS: "); kernel_print_hex(regs.ds);    kernel_print(" SS: "); kernel_print_hex(regs.ss);
    kernel_print("\n");
    kernel_print("  EFLAGS: "); kernel_print_hex(regs.eflags);
    kernel_print("\n");
#endif

    // Verifica se existe uma função registrada no nosso array para essa interrupção.
    if (interrupt_handlers[regs.int_no] != 0) {
        // Se existe, pega o ponteiro da função.
        isr_t handler = interrupt_handlers[regs.int_no];
        // Chama a função, passando para ela o endereço de memória da struct 'regs'.
        handler(&regs);
    } else {
        // Se não houver um manipulador registrado, avisa no console.
        // This part is now redundant due to the print statements above, but we can keep it for clarity
        // or remove the specific "Unhandled interrupt" message if the detailed log is sufficient.
        // For now, let's keep it to explicitly state it's unhandled by a specific routine.
        kernel_print(" -> No specific handler registered.\n");
    }
}

// Função para registrar um manipulador (callback) para uma interrupção específica.
void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
