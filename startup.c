#include <stdint.h>

/* Prototypes */
extern int main(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

/* Default handler */
void Reset_Handler(void);
void Default_Handler(void);

/* Stack */
#define STACK_SIZE 1024
static uint32_t stack[STACK_SIZE] __attribute__((section(".stack")));

/* Vetor de interrupções */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))((uint32_t)stack + sizeof(stack)),
    Reset_Handler,
    Default_Handler,
    Default_Handler,
    0, 0, 0, 0, 0, 0, 0,
    SVC_Handler,         // CORRETO
    Default_Handler,
    0,
    PendSV_Handler,      // CORRETO
    SysTick_Handler      // CORRETO
};



/* UART0 simulação (opcional, mas bom ter para printf via QEMU) */
void uart0_init(void)
{
    // QEMU já inicia UART0 automaticamente, então isso pode ficar vazio
}

/* Reset Handler de verdade */
void Reset_Handler(void)
{
    extern uint32_t _data, _edata, _etext, _bss, _ebss;

    uint32_t *src = &_etext;
    uint32_t *dst = &_data;

    // Copia dados inicializados da FLASH para RAM
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zera a BSS (variáveis globais não inicializadas)
    for (dst = &_bss; dst < &_ebss; ++dst) {
        *dst = 0;
    }

    uart0_init(); // Apenas simbólico

    main(); // 🔥 CHAMA O MAIN()!
}

/* Handler padrão */
void Default_Handler(void)
{
    while (1);
}
