/*
 * kernel/kernel.c - O kernel principal do StarOS
 */
#include <kernel/types.h>
#include <kernel/vga.h>
#include <libs/graphics.h>
#include <drivers/mouse.h>
#include <drivers/keyboard.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/isr.h>
#include <kernel/ports.h> // ADICIONADO: Necessário para inb/outb

// --- Variáveis Globais para o Modo Texto ---
volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;
int text_color = 0x0F;
int cursor_x = 0;
int cursor_y = 0;

// --- Funções de Controle do Cursor VGA ---

// NOVO: Função para atualizar a posição do cursor de hardware
void vga_update_cursor(int x, int y) {
    unsigned short position = (y * VGA_WIDTH) + x;

    // Envia comando para a porta de controle do VGA para dizer qual registro vamos setar
    // 0x0F para o byte baixo da posição, 0x0E para o byte alto
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

// --- Funções de Impressão ---

void kernel_clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (unsigned short)' ' | (unsigned short)text_color << 8;
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor(cursor_x, cursor_y); // ATUALIZADO: Move o cursor para o início
}

void kernel_print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        // CORREÇÃO 1: Lógica do Backspace
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            // Se estamos no começo da linha, sobe para a linha anterior
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        // Apaga o caractere na nova posição
        const int index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = (unsigned short)' ' | (unsigned short)text_color << 8;

    } else {
        const int index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = (unsigned short)c | (unsigned short)text_color << 8;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    vga_update_cursor(cursor_x, cursor_y);
}

void kernel_print(const char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        kernel_print_char(str[i]);
    }
}

// --- Manipuladores de Interrupção ---

// ATUALIZADO: Handler do Timer com o aviso do compilador corrigido
static void timer_callback(registers_t* regs) {
    (void)regs; // Diz ao compilador que 'regs' não ser usado é intencional.
    pic_send_eoi(0); // 0 é o IRQ do Timer
}

// --- Função Principal do Kernel ---

extern void sti();

static void keyboard_dummy_callback(registers_t* regs) {
    (void)regs;
    // Precisamos ler a porta para o controlador do teclado ficar feliz,
    // mas não fazemos nada com o valor.
    inb(0x60);
    // Enviamos o EOI para o PIC para não travar o sistema.
    pic_send_eoi(1);
}

void kernel_main(void) {
    // Inicializa a infraestrutura básica de interrupções
    idt_init();
    pic_remap(0x20, 0x28);
    
    // CORREÇÃO: Registra o handler dummy do teclado para a interrupção 33 (IRQ 1)
    register_interrupt_handler(33, keyboard_dummy_callback);

    // Inicializa os drivers
    keyboard_init(); // Prepara o handler dummy do teclado
    mouse_init();    // Prepara o handler do mouse

    // Habilita TODAS as interrupções de hardware
    sti();

    set_vga_mode_320x200();

    int old_mx = 160, old_my = 100;
    int mx = 160, my = 100;

    // Pinta a tela inteira de azul escuro (Cor 1)
    fill_rect(0, 0, 320, 200, 1);

    // Loop principal do sistema operacional gráfico
    while(1) {
        // Apaga o cursor antigo (desenhando o fundo azul por cima)
        fill_rect(old_mx, old_my, 3, 3, 1);

        mx = get_mouse_x();
        my = get_mouse_y();

        // Desenha o novo cursor (um quadrado branco 3x3)
        fill_rect(mx, my, 3, 3, 15); // Cor 15 = Branco Brilhante

        old_mx = mx;
        old_my = my;
    }
}

// Helper function to print a number in hexadecimal
void kernel_print_hex(u32 n) {
    char hex_chars[] = "0123456789ABCDEF";
    kernel_print("0x");
    if (n == 0) {
        kernel_print_char('0');
        return;
    }
    // Print nibbles from most significant to least significant
    for (int i = 7; i >= 0; i--) {
        unsigned char nibble = (n >> (i * 4)) & 0xF;
        kernel_print_char(hex_chars[nibble]);
    }
}

// Helper function to print a number in decimal
void kernel_print_dec(u32 n) {
    if (n == 0) {
        kernel_print_char('0');
        return;
    }
    char buffer[11]; // Max 10 digits for u32 + null terminator
    int i = 0;
    while (n > 0) {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }
    buffer[i] = '\0';
    // Reverse the buffer
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    kernel_print(buffer);
}
