# Makefile para o StarOS

# Ferramentas
ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

# Flags
ASMFLAGS = -f elf32
# Adicionamos -I para os diretórios de headers
CFLAGS = -m32 -ffreestanding -c -g -Wall -Wextra -I. -Ikernel -Ilibs -Idrivers
LDFLAGS = -m elf_i386 -T linker.ld

# Diretórios
BUILD_DIR = build
SRC_DIRS = kernel libs drivers

# Encontra todos os arquivos .asm e .c nos diretórios de origem
AS_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.asm))
C_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Gera os nomes dos arquivos objeto
AS_OBJS = $(patsubst %.asm, $(BUILD_DIR)/%.o, $(AS_SOURCES))
C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJS = $(AS_OBJS) $(C_OBJS)

# Alvo principal
all: $(BUILD_DIR)/staros.elf

# Linka os arquivos objeto
$(BUILD_DIR)/staros.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Regra genérica para compilar C
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

# Regra genérica para compilar Assembly
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $< -o $@

# Limpa os arquivos de build
clean:
	rm -rf $(BUILD_DIR)

# Regra para rodar com QEMU (emulador)
run: all
	qemu-system-i386 -kernel $(BUILD_DIR)/staros.elf

.PHONY: all clean run