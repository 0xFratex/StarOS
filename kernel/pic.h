// kernel/pic.h - Interface para o Controlador de Interrupções Programável (PIC)

#ifndef PIC_H
#define PIC_H

// Remapeia o PIC para que os IRQs de hardware não conflitem com as exceções da CPU.
// IRQs 0-7 são mapeados para offset1.
// IRQs 8-15 são mapeados para offset2.
void pic_remap(int offset1, int offset2);

// Envia o comando de Fim de Interrupção (End of Interrupt) para o PIC.
// Isso é necessário no final de cada manipulador de IRQ.
void pic_send_eoi(unsigned char irq);

#endif