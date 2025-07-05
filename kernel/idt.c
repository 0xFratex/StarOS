// kernel/idt.c - Implementação da Tabela de Descritores de Interrupção (IDT)

#include <kernel/idt.h>
#include <kernel/types.h>

// Declarações externas para os stubs das ISRs definidos em boot.asm.
// O compilador C precisa saber que essas funções existem em outro lugar.

// Exceções da CPU (0-31)
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// IRQs de Hardware (32-47)
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();


// Função externa (em Assembly) para carregar a IDT
extern void idt_load();

// Array de 256 entradas da IDT e o ponteiro para ela
struct idt_entry idt[256];
struct idt_ptr idtp;

// Define uma única entrada na Tabela de Descritores de Interrupção
void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Instala todas as rotinas de serviço de interrupção (ISRs) na IDT
void isr_install() {
    // 0x08 é o seletor do segmento de código do kernel
    // 0x8E são as flags para um "Interrupt Gate" de 32 bits presente

    // Exceções da CPU
    idt_set_gate(0, (u32)isr0, 0x08, 0x8E);
    idt_set_gate(1, (u32)isr1, 0x08, 0x8E);
    idt_set_gate(2, (u32)isr2, 0x08, 0x8E);
    idt_set_gate(3, (u32)isr3, 0x08, 0x8E);
    idt_set_gate(4, (u32)isr4, 0x08, 0x8E);
    idt_set_gate(5, (u32)isr5, 0x08, 0x8E);
    idt_set_gate(6, (u32)isr6, 0x08, 0x8E);
    idt_set_gate(7, (u32)isr7, 0x08, 0x8E);
    idt_set_gate(8, (u32)isr8, 0x08, 0x8E);
    idt_set_gate(9, (u32)isr9, 0x08, 0x8E);
    idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32)isr31, 0x08, 0x8E);

    // IRQs (remapeadas pelo PIC)
    idt_set_gate(32, (u32)isr32, 0x08, 0x8E); // IRQ0: System Timer
    idt_set_gate(33, (u32)isr33, 0x08, 0x8E); // IRQ1: Keyboard
    idt_set_gate(34, (u32)isr34, 0x08, 0x8E); // IRQ2: Cascade
    idt_set_gate(35, (u32)isr35, 0x08, 0x8E); // IRQ3: COM2
    idt_set_gate(36, (u32)isr36, 0x08, 0x8E); // IRQ4: COM1
    idt_set_gate(37, (u32)isr37, 0x08, 0x8E); // IRQ5: LPT2
    idt_set_gate(38, (u32)isr38, 0x08, 0x8E); // IRQ6: Floppy Disk
    idt_set_gate(39, (u32)isr39, 0x08, 0x8E); // IRQ7: LPT1
    idt_set_gate(40, (u32)isr40, 0x08, 0x8E); // IRQ8: CMOS real-time clock
    idt_set_gate(41, (u32)isr41, 0x08, 0x8E); // IRQ9
    idt_set_gate(42, (u32)isr42, 0x08, 0x8E); // IRQ10
    idt_set_gate(43, (u32)isr43, 0x08, 0x8E); // IRQ11
    idt_set_gate(44, (u32)isr44, 0x08, 0x8E); // IRQ12: PS/2 Mouse
    idt_set_gate(45, (u32)isr45, 0x08, 0x8E); // IRQ13: FPU
    idt_set_gate(46, (u32)isr46, 0x08, 0x8E); // IRQ14: Primary ATA Hard Disk
    idt_set_gate(47, (u32)isr47, 0x08, 0x8E); // IRQ15: Secondary ATA Hard Disk
}

// Inicializa a IDT
void idt_init() {
    // Configura o ponteiro da IDT (limite e endereço base)
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (u32)&idt;

    // Zera a IDT para começar (uma boa prática)
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Preenche a IDT com os stubs das ISRs
    isr_install();

    // Carrega a IDT na CPU usando a instrução 'lidt' em Assembly
    idt_load();
}