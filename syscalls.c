#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include "FreeRTOS.h"
#include "task.h"

// UART0 (simulado no QEMU)
#define UART0_DR (*(volatile uint32_t *)0x4000C000)
#define UART0_FR (*(volatile uint32_t *)0x4000C018)
#define UART_FR_TXFF 0x20

// Para usar printf com QEMU
int _write(int file, char *ptr, int len) {
    (void)file;
    for (int i = 0; i < len; i++) {
        while (UART0_FR & UART_FR_TXFF); // Espera espaço no buffer
        UART0_DR = ptr[i];
    }
    return len;
}

// Implementações mínimas exigidas pela Newlib
int _close(int file) { (void)file; return -1; }

int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file; (void)ptr; (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len) {
    (void)file; (void)ptr; (void)len;
    return 0;
}

int _kill(int pid, int sig) {
    (void)pid; (void)sig;
    return -1;
}

int _getpid(void) {
    return 1;
}

void _exit(int status) {
    (void)status;
    while (1);
}

// Implementação de heap para malloc/free
extern char _end; // Fornecido pelo linker script
static char *heap_end = &_end;

void* _sbrk(ptrdiff_t incr) {
    char *prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}

int _open(const char *name, int flags, int mode) {
    (void)name;
    (void)flags;
    (void)mode;
    return -1; // Retorna erro pois não há sistema de arquivos
}


// Hooks exigidos pelo FreeRTOS
void vApplicationMallocFailedHook(void) {
    // Aqui pode piscar LED, logar erro, etc
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    // Aqui pode logar o nome da task com stack overflow
    while (1);
}
