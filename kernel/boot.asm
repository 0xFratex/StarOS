; kernel/boot.asm - Ponto de entrada e stubs de interrupção

; --- Cabeçalho Multiboot ---
MBALIGN  equ  1<<0
MEMINFO  equ  1<<1
MAGIC    equ  0x1BADB002
FLAGS    equ  MBALIGN | MEMINFO
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; --- Seção de Texto (Código) ---
section .text
bits 32

global start
extern kernel_main
extern isr_handler ; Função C que trata as interrupções

; Stubs das ISRs (Manipuladores de Serviço de Interrupção)
; Os 32 primeiros são para exceções da CPU
global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
; Os próximos são para IRQs de hardware
global isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39, isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47

%macro ISR_NOERRCODE 1
isr%1:
    cli
    push 0  ; Empilha um código de erro falso
    push %1 ; Empilha o número da interrupção
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
isr%1:
    cli
    ; O código de erro já está na pilha
    push %1 ; Empilha o número da interrupção
    jmp isr_common_stub
%endmacro

; Stubs para exceções da CPU
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
%assign i 21
%rep 11
    ISR_NOERRCODE i
%assign i i+1
%endrep

; Stubs para IRQs (Hardware)
%assign i 32
%rep 16
    ISR_NOERRCODE i
%assign i i+1
%endrep

; Stub comum que salva o estado da CPU e chama o handler em C
isr_common_stub:
    pusha       ; Salva edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds
    push eax    ; Salva o segmento de dados

    mov ax, 0x10 ; Carrega o seletor de segmento de dados do kernel
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler ; Chama o handler em C

    pop eax     ; Restaura o segmento de dados original
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa        ; Restaura os registradores
    add esp, 8  ; Limpa o código de erro e o número da interrupção da pilha
    sti         ; Reabilita as interrupções
    iret        ; Retorna da interrupção

start:
    mov esp, stack_space
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang

; Função para carregar a IDT
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

; Função para habilitar interrupções
global sti
sti:
    sti
    ret

section .bss
resb 8192
stack_space: