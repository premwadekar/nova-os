/* kernel.c -- NOVA OS kernel entry (Day 2: GDT added) */

#include "arch/gdt.h"

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
        vga[offset]     = msg[i];           /* character byte */
        vga[offset + 1] = COLOR_DEFAULT;    /* color byte */
        i++;
    }
}

void kernel_main(void) {
    print_string("NOVA OS 1.0 - Kernel initialized successfully.", current_row++);

    /* GDT install karo - CPU ab humare custom GDT pe chalega */
    gdt_install();

    print_string("GDT loaded successfully.", current_row++);

    while (1) {
        /* Kernel yahan infinite loop mein rahega for now */
    }
}
