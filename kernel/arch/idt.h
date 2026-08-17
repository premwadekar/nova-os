#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* Ek IDT entry (interrupt gate descriptor) -- 8 bytes */
struct idt_entry {
    uint16_t base_low;    // Handler address ke pehle 16 bits
    uint16_t sel;         // GDT mein kaunsa code segment use karna hai (0x08 = kernel code)
    uint8_t  always0;     // Hamesha 0 rehta hai
    uint8_t  flags;       // Type aur attributes (present, ring, gate type)
    uint16_t base_high;   // Handler address ke last 16 bits
} __attribute__((packed));

/* IDT Pointer -- CPU ko batata hai IDT kahan hai aur kitni badi hai */
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Ye function kernel.c se call hoga IDT setup karne ke liye */
void idt_install(void);

/* Ek entry set karne ke liye helper (idt.c ke andar use hoga, isr.c se bhi call hoga) */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
