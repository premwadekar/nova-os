#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/* Jab interrupt aata hai, ye poora structure stack pe hota hai --
   humare assembly stub ne registers push kiye, phir CPU ne kuch push kiya */
typedef struct registers {
    uint32_t ds;                                      // Data segment pushed by us
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;   // Pushed by 'pusha'
    uint32_t int_no, err_code;                         // Interrupt number + error code (agar hai)
    uint32_t eip, cs, eflags, useresp, ss;              // Automatically pushed by CPU
} registers_t;

void isrs_install(void);

/* Common C handler -- saare ISR stubs isse call karte hain */
void isr_handler(registers_t *regs);

/* Common C handler -- saare IRQ stubs isse call karte hain (irq.c mein defined) */
void irq_handler(registers_t *regs);

#endif
