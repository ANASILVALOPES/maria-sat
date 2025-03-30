# Definindo os arquivos fonte e objetos
SRC = src/main.c src/MAIN_SO.c FreeRTOS-Kernel/portable/GCC/Posix/port.c
OBJ = $(SRC:.c=.o)

# Diretórios de inclusão
CFLAGS := -Wall -Wextra -g -I$(INCLUDE_DIR) -I$(FREERTOS_DIR)/include -I$(FREERTOS_DIR)/portable/GCC/Posix

# Nome final do executável
TARGET = maria-sat.elf

# Regras de compilação
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Compilação dos arquivos .c em .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJ) $(TARGET)
