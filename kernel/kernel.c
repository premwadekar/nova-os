/* kernel.c -- NOVA OS kernel entry (Day 2: GDT + IDT + ISR + IRQ + Keyboard added) */

#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/isr.h"
#include "arch/irq.h"
#include "drivers/keyboard.h"

/* VGA text mode buffer starts here */
#define VGA_MEMORY ((unsigned char*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* Standard VGA color: light grey text on black background */
#define COLOR_DEFAULT 0x07

/* Current row - global taaki multiple print calls alag lines pe likhein */
static int current_row = 0;

/* Helper function: ek string ko VGA buffer mein di gayi row pe print karta hai */
void print_string(const char *msg, int row) {
    unsigned char *vga = VGA_MEMORY;
    int i = 0;
    while (msg[i] != '\0') {
        int offset = (row * VGA_WIDTH + i) * 2;
        vga[offset]     = msg[i];       /* character byte */
        vga[offset + 1] = COLOR_DEFAULT; /* color byte */
        i++;
    }
}

void kernel_main(void) {
    print_string("NOVA OS 1.0 - Kernel initialized successfully.", current_row++);

    /* GDT install karo - CPU ab humare custom GDT pe chalega */
    gdt_install();
    print_string("GDT loaded successfully.", current_row++);

    /* IDT install karo - interrupt table zero se initialize hoti hai */
    idt_install();
    print_string("IDT loaded successfully.", current_row++);

    /* ISR handlers install karo - CPU exceptions (0-31) ke liye */
    isrs_install();
    print_string("ISR handlers installed successfully.", current_row++);

    /* IRQ handlers install karo - PIC remap + hardware interrupts (32-47) ke liye */
    irq_install();
    print_string("IRQ (PIC) installed successfully.", current_row++);

    /* Keyboard driver install karo - IRQ1 ke liye handler register karta hai */
    keyboard_install();
    print_string("Keyboard driver installed successfully.", current_row++);

    /* Interrupts ko globally enable karo -- ab tak CPU interrupts ignore kar raha tha */
    __asm__ __volatile__("sti");
    print_string("Interrupts enabled. Try typing on your keyboard!", current_row++);

    while (1) {
        /* Kernel yahan infinite loop mein rahega, ab keyboard interrupts handle honge */
    }
}
