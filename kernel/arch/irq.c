/* irq.c -- PIC remapping + hardware interrupt (IRQ) dispatch */

#include "irq.h"
#include "idt.h"
#include "isr.h"

/* I/O port helper functions -- x86 mein hardware se baat karne ke liye
   inb/outb assembly instructions use hote hain */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* PIC I/O ports */
#define PIC1        0x20   /* Master PIC command port */
#define PIC1_DATA   0x21   /* Master PIC data port */
#define PIC2        0xA0   /* Slave PIC command port */
#define PIC2_DATA   0xA1   /* Slave PIC data port */

#define PIC_EOI     0x20   /* End Of Interrupt command */

/* External assembly stubs for IRQ0-15 (irq_stubs.s mein defined honge) */
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

/* Har IRQ ke liye ek optional handler function pointer -- default NULL */
static irq_handler_t irq_routines[16] = { 0 };

void irq_install_handler(int irq, irq_handler_t handler) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

/* PIC ko remap karta hai -- IRQ0-15 ko interrupt 32-47 pe move karta hai */
static void pic_remap(void) {
    /* Save masks (abhi humein zaroorat nahi, but future-safe) */
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1, 0x11);   /* ICW1: initialization mode start */
    outb(PIC2, 0x11);

    outb(PIC1_DATA, 0x20);  /* ICW2: Master PIC ab 0x20 (32) se start hoga */
    outb(PIC2_DATA, 0x28);  /* ICW2: Slave PIC ab 0x28 (40) se start hoga */

    outb(PIC1_DATA, 0x04);  /* ICW3: batao master ko, slave PIC IRQ2 pe hai */
    outb(PIC2_DATA, 0x02);  /* ICW3: batao slave ko, apni cascade identity */

    outb(PIC1_DATA, 0x01);  /* ICW4: 8086 mode */
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, mask1); /* purane masks restore karo */
    outb(PIC2_DATA, mask2);
}

void irq_install(void) {
    pic_remap();

    idt_set_gate(32, (uint32_t)irq0,  0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2,  0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3,  0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4,  0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5,  0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6,  0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7,  0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8,  0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9,  0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}

/* Ye C function saare IRQ stubs se call hota hai */
void irq_handler(registers_t *regs) {
    /* Agar handler registered hai is IRQ ke liye, use call karo */
    if (regs->int_no >= 32 && regs->int_no < 48) {
        int irq_num = regs->int_no - 32;
        if (irq_routines[irq_num]) {
            irq_routines[irq_num]();
        }
    }

    /* Slave PIC ko bhi EOI bhejna hai agar IRQ >= 8 tha */
    if (regs->int_no >= 40) {
        outb(PIC2, PIC_EOI);
    }
    /* Master PIC ko hamesha EOI bhejna hai */
    outb(PIC1, PIC_EOI);
}
