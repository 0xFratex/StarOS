// kernel/pic.c - Código para controlar o PIC 8259

#include <kernel/ports.h>

// Portas do PIC
#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA   (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA   (PIC2+1)

// Comando de Fim de Interrupção (End of Interrupt)
#define PIC_EOI     0x20

// Remapeia o PIC para que os IRQs não conflitem com as exceções da CPU
void pic_remap(int offset1, int offset2) {
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA); // Salva as máscaras
    a2 = inb(PIC2_DATA);

    // Inicia a sequência de inicialização (em modo cascata)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // Define os offsets dos vetores do PIC
    outb(PIC1_DATA, offset1); // PIC1 começa em 32
    outb(PIC2_DATA, offset2); // PIC2 começa em 40

    // Informa ao PIC1 que há um PIC2 na IRQ2
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    // Define o modo 8086
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Restaura as máscaras salvas
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

// Envia o comando EOI para o PIC correto
void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}