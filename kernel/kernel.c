/* kernel.c -- NOVA OS kernel entry (Day 1: simplest version) */

/* VGA text mode buffer starts here */
#define VGA_MEMORY ((unsigned char*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* Standard VGA color: light grey text on black background */
#define COLOR_DEFAULT 0x07

void kernel_main(void) {
    const char *msg = "NOVA OS 1.0 - Kernel initialized successfully.";
    unsigned char *vga = VGA_MEMORY;

    int i = 0;
    while (msg[i] != '\0') {
        vga[i * 2]     = msg[i];        /* character byte */
        vga[i * 2 + 1] = COLOR_DEFAULT; /* color byte */
        i++;
    }
}
