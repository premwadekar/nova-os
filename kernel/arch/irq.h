#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

/* IRQ handler function pointer type -- future mein keyboard/timer isse register karenge */
typedef void (*irq_handler_t)(void);

void irq_install(void);
void irq_install_handler(int irq, irq_handler_t handler);
void irq_uninstall_handler(int irq);

#endif
