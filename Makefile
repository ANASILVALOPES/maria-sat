# Diretórios do projeto
PROJECT_DIR := $(shell pwd)
SRC_DIR := $(PROJECT_DIR)/src
INCLUDE_DIR := $(PROJECT_DIR)/include
FREERTOS_DIR := $(PROJECT_DIR)/FreeRTOS-Kernel
BUILD_DIR := $(PROJECT_DIR)/build

# Nome do arquivo de saída
OUTPUT := maria-sat.elf

# Compilador para ARM Cortex-M3
CC := arm-none-eabi-gcc

# Flags de compilação
CFLAGS := -mcpu=cortex-m3 -mthumb -Wall -Wextra -g \
	-I$(INCLUDE_DIR) \
	-I$(FREERTOS_DIR)/include \
	-I$(FREERTOS_DIR)/portable/GCC/ARM_CM3 \
	-DARM_CM3

# Flags de linkagem
LDFLAGS := -T linker.ld -nostartfiles -Wl,--gc-sections

# Fontes do FreeRTOS + fontes do projeto
FREERTOS_SRC := $(wildcard $(FREERTOS_DIR)/*.c) \
                $(wildcard $(FREERTOS_DIR)/portable/GCC/ARM_CM3/*.c)

SRC_FILES := $(shell find $(SRC_DIR) -name "*.c") $(FREERTOS_SRC) syscalls.c startup.c FreeRTOS-Kernel/portable/MemMang/heap_4.c

# Arquivos objeto
OBJ_FILES := $(SRC_FILES:$(PROJECT_DIR)/%.c=$(BUILD_DIR)/%.o)

# Garante que a pasta build existe
$(shell mkdir -p $(BUILD_DIR))

# Regra padrão
all: $(OUTPUT)

# Linkagem
$(OUTPUT): $(OBJ_FILES)
	@echo "Arquivos objeto sendo vinculados: $(OBJ_FILES)"
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(OUTPUT) $(LDFLAGS)

# Compilação
$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compilando $< para $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT)

clear: clean

# Simulação QEMU
run: $(OUTPUT)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(OUTPUT)