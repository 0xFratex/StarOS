/* linker.ld - Script de linkagem para o StarOS */
ENTRY(start) /* Define o ponto de entrada */

SECTIONS
{
    . = 1M; /* O kernel será carregado em 1MB de memória */

    .text :
    {
        *(.multiboot) /* Seção para o cabeçalho Multiboot */
        *(.text)     /* Seção de código */
    }

    .rodata :
    {
        *(.rodata)   /* Seção de dados somente leitura */
    }

    .data :
    {
        *(.data)     /* Seção de dados inicializados */
    }

    .bss :
    {
        *(.bss)      /* Seção de dados não inicializados */
    }
}
