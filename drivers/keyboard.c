// drivers/keyboard.c

#include <drivers/keyboard.h>
#include <kernel/ports.h>
#include <kernel/isr.h>
#include <kernel/pic.h>
#include <kernel/kernel.h>

// Scancode Set 1 (US QWERTY) para caracteres imprimíveis
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Função que será chamada pela interrupção do teclado
static void keyboard_dummy_handler(registers_t* regs) {
    (void)regs; // Silencia o aviso de parâmetro não usado

    // É importante ler a porta 0x60 para resetar o hardware do teclado,
    // senão ele pode continuar enviando a mesma interrupção.
    inb(0x60);

    // Avisa ao PIC que terminamos de tratar a interrupção IRQ 1.
    pic_send_eoi(1);
}

void keyboard_init() {
    // Registra nosso handler dummy para a interrupção 33 (IRQ 1).
    register_interrupt_handler(33, keyboard_dummy_handler);
}