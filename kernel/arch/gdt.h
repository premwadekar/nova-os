#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// Ek GDT entry (segment descriptor) - 8 bytes
// __attribute__((packed)) zaroori hai taaki compiler
// padding add na kare - hume EXACT byte layout chahiye
// jo CPU expect karta hai
struct gdt_entry {
    uint16_t limit_low;     // Limit ke pehle 16 bits
    uint16_t base_low;      // Base address ke pehle 16 bits
    uint8_t  base_middle;   // Base address ke agle 8 bits
    uint8_t  access;        // Access flags (present, ring, type)
    uint8_t  granularity;   // Granularity + limit ke last 4 bits
    uint8_t  base_high;     // Base address ke last 8 bits
} __attribute__((packed));

// GDT Pointer - ye structure CPU ko batata hai
// GDT kahan hai memory mein aur kitni badi hai
struct gdt_ptr {
    uint16_t limit;   // GDT ka total size - 1 (bytes mein)
    uint32_t base;    // GDT ka starting memory address
} __attribute__((packed));

// Ye function kernel.c se call hoga GDT setup karne ke liye
void gdt_install(void);

#endif
