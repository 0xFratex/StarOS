// drivers/mouse.c - Driver para o mouse PS/2

#include <drivers/mouse.h>
#include <kernel/ports.h>
#include <kernel/isr.h>
#include <kernel/pic.h>
#include <libs/graphics.h> // Para desenhar o cursor

// Posição e estado do mouse
static int mouse_x = 160;
static int mouse_y = 100;
static int mouse_cycle = 0;
static char mouse_packet[3];

// Função para esperar o dispositivo PS/2 estar pronto para receber dados
void mouse_wait(int type) {
    unsigned int time_out = 100000;
    if (type == 0) { // Esperar para enviar
        while (time_out--) {
            if ((inb(0x64) & 2) == 0) return;
        }
    } else { // Esperar para receber
        while (time_out--) {
            if ((inb(0x64) & 1) != 0) return;
        }
    }
}

// Envia um comando para o mouse
void mouse_write(unsigned char a_write) {
    mouse_wait(0);
    outb(0x64, 0xD4);
    mouse_wait(0);
    outb(0x60, a_write);
}

// Callback que é chamado pela interrupção do mouse (IRQ 12)
static void mouse_callback(registers_t* regs) {
    (void)regs;

    // Lê o pacote de dados do mouse
    switch(mouse_cycle) {
        case 0:
            mouse_packet[0] = inb(0x60);
            // Checa se é o primeiro byte (bit 3 deve ser 1)
            if ((mouse_packet[0] & 0x08)) {
                mouse_cycle++;
            }
            break;
        case 1:
            mouse_packet[1] = inb(0x60);
            mouse_cycle++;
            break;
        case 2:
            mouse_packet[2] = inb(0x60);
            mouse_cycle = 0;

            // Decodifica o pacote
            int delta_x = mouse_packet[1];
            int delta_y = mouse_packet[2];

            // Trata o sinal de overflow e direção
            if ((mouse_packet[0] & 0x10)) delta_x |= 0xFFFFFF00;
            if ((mouse_packet[0] & 0x20)) delta_y |= 0xFFFFFF00;

            mouse_x += delta_x;
            mouse_y -= delta_y; // O eixo Y do mouse é invertido

            // Limita o cursor à tela
            if (mouse_x < 0) mouse_x = 0;
            if (mouse_x > 319) mouse_x = 319;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_y > 199) mouse_y = 199;
            break;
    }

    pic_send_eoi(12);
}

void mouse_init() {
    unsigned char status;

    // Habilita o segundo dispositivo PS/2 (mouse)
    mouse_wait(0);
    outb(0x64, 0xA8);

    // Habilita as interrupções
    mouse_wait(0);
    outb(0x64, 0x20);
    mouse_wait(1);
    status = (inb(0x60) | 2);
    mouse_wait(0);
    outb(0x64, 0x60);
    mouse_wait(0);
    outb(0x60, status);

    // Define os padrões do mouse
    mouse_write(0xF6);
    mouse_wait(1);
    inb(0x60); // ACK

    // Habilita o envio de pacotes
    mouse_write(0xF4);
    mouse_wait(1);
    inb(0x60); // ACK

    // Registra o handler para a interrupção 44 (IRQ 12)
    register_interrupt_handler(44, mouse_callback);
}

// Funções para obter a posição do mouse
int get_mouse_x() { return mouse_x; }
int get_mouse_y() { return mouse_y; }