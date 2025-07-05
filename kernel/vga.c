// kernel/vga.c - Implementação do controle de vídeo

#include <kernel/vga.h>

void set_vga_mode_320x200() {
    // Usamos uma chamada de interrupção da BIOS (int 0x10) para mudar o modo de vídeo.
    // O modo 0x13 é o modo gráfico 320x200x256.
    __asm__ __volatile__ (
        "mov $0x13, %%al\n\t"
        "mov $0x00, %%ah\n\t"
        "int $0x10"
        : : : "eax"
    );
}