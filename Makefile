# Diretórios do projeto
PROJECT_DIR := $(shell pwd)
SRC_DIR := $(PROJECT_DIR)/src
INCLUDE_DIR := $(PROJECT_DIR)/include
FREERTOS_DIR := $(PROJECT_DIR)/FreeRTOS-Kernel
PORT_DIR := $(FREERTOS_DIR)/portable/GCC/Posix
BUILD_DIR := $(PROJECT_DIR)/build

# Nome do arquivo de saída
OUTPUT := maria-sat.elf

# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -g \
	-I$(INCLUDE_DIR) \
	-I$(FREERTOS_DIR)/include \
	-I$(PORT_DIR) \
	-DFREERTOS_POSIX \
	-DportUSING_POSIX \
	-DTickType_t=uint32_t

# Arquivos fonte
FREERTOS_SRC := $(wildcard $(FREERTOS_DIR)/*.c) \
				$(wildcard $(PORT_DIR)/*.c)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) $(FREERTOS_SRC)

# Arquivos objeto (mantendo estrutura de pastas no build)
OBJ_FILES := $(patsubst $(PROJECT_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Criação da pasta build caso não exista
$(shell mkdir -p $(BUILD_DIR))

# Tarefa padrão
all: $(OUTPUT)

# Linkando os arquivos objeto
$(OUTPUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(OUTPUT)

# Compilação dos arquivos fonte (com suporte a subpastas)
$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza de arquivos temporários
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT)

# Alias para limpeza
clear: clean

# Regra para rodar o programa (opcional)
run: $(OUTPUT)
	./$(OUTPUT)
