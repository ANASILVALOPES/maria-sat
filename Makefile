# Diretórios do projeto
PROJECT_DIR := $(shell pwd)
SRC_DIR := $(PROJECT_DIR)/src
INCLUDE_DIR := $(PROJECT_DIR)/include
FREERTOS_DIR := $(PROJECT_DIR)/FreeRTOS-Kernel
BUILD_DIR := $(PROJECT_DIR)/build

# Nome do arquivo de saída
OUTPUT := maria-sat.elf

# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -g \
	-I$(INCLUDE_DIR) \
	-I$(FREERTOS_DIR)/include \
	-I$(FREERTOS_DIR)/portable/GCC/Posix \
	-DportUSING_POSIX


# Arquivos fonte
FREERTOS_SRC := $(wildcard $(FREERTOS_DIR)/*.c) \
				$(wildcard $(FREERTOS_DIR)/portable/GCC/Posix/*.c)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) $(FREERTOS_SRC)

# Arquivos objeto
OBJ_FILES := $(SRC_FILES:$(PROJECT_DIR)/%.c=$(BUILD_DIR)/%.o)

# Criação da pasta build caso não exista
$(shell mkdir -p $(BUILD_DIR))

# Tarefa padrão
all: $(OUTPUT)

# Linkando os arquivos objeto
$(OUTPUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(OUTPUT)

# Compilação dos arquivos fonte (com suporte a subdiretórios)
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