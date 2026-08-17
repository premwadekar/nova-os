/* keyboard.c -- PS/2 keyboard driver (IRQ1) -- DEBUG VERSION */

#include "keyboard.h"
#include "../arch/irq.h"
#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#define KEYBOARD_DATA_PORT 0x60

#define VGA_MEMORY ((unsigned char*)0xB8000)
#define VGA_WIDTH  80
#define COLOR_DEFAULT 0x07
#define COLOR_DEBUG   0x0E  /* yellow -- debug info ke liye alag color */

static int kb_row = 12;
static int kb_col = 0;

/* Debug: har baar keyboard_handler call hote hi ye counter badhega */
static volatile int debug_call_count = 0;

static char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\', 'z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};

static void putchar_screen(char c, int row, int col, unsigned char color) {
    unsigned char *vga = VGA_MEMORY;
    int offset = (row * VGA_WIDTH + col) * 2;
    vga[offset]     = c;
    vga[offset + 1] = color;
}

/* Debug helper: ek byte ko 2-digit hex string ki tarah print karta hai */
static void print_hex_byte(uint8_t val, int row, int col) {
    const char *hex = "0123456789ABCDEF";
    putchar_screen(hex[(val >> 4) & 0xF], row, col, COLOR_DEBUG);
    putchar_screen(hex[val & 0xF], row, col + 1, COLOR_DEBUG);
}

static void print_int(int val, int row, int col) {
    char buf[6];
    int i = 0;
    if (val == 0) { putchar_screen('0', row, col, COLOR_DEBUG); return; }
    while (val > 0 && i < 5) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    for (int j = 0; j < i; j++) {
        putchar_screen(buf[j], row, col + (i - 1 - j), COLOR_DEBUG);
    }
}

static void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    debug_call_count++;

    /* DEBUG: har baar handler call hone pe counter aur scancode dikhao,
       chahe key press ho ya release -- taaki confirm ho IRQ fire ho raha hai */
    putchar_screen('C', 20, 0, COLOR_DEBUG);
    putchar_screen(':', 20, 1, COLOR_DEBUG);
    print_int(debug_call_count, 20, 2);

    putchar_screen('S', 20, 10, COLOR_DEBUG);
    putchar_screen(':', 20, 11, COLOR_DEBUG);
    putchar_screen('0', 20, 12, COLOR_DEBUG);
    putchar_screen('x', 20, 13, COLOR_DEBUG);
    print_hex_byte(scancode, 20, 14);

    if (scancode & 0x80) {
        return;
    }

    char c = scancode_to_ascii[scancode];
    if (c != 0) {
        putchar_screen(c, kb_row, kb_col, COLOR_DEFAULT);
        kb_col++;
        if (kb_col >= VGA_WIDTH) { kb_col = 0; kb_row++; }
    }
}

void keyboard_install(void) {
    irq_install_handler(1, keyboard_handler);
}
