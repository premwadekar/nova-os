#include "gdt.h"

// GDT mein 5 entries: null, kernel code, kernel data, user code, user data
struct gdt_entry gdt[5];
struct gdt_ptr   gp;

// Ye Assembly function hai (gdt_flush.s mein defined)
// Isse CPU ko bolte hain "naya GDT use karo"
extern void gdt_flush(uint32_t);

// Helper function - ek GDT entry (descriptor) fill karta hai
// num       -> kaunsi entry (0 se 4)
// base      -> segment ka starting address
// limit     -> segment ka size
// access    -> access byte (permissions, ring level, type)
// gran      -> granularity byte (4KB blocks vs bytes, 32-bit mode)
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit,
                          uint8_t access, uint8_t gran) {

    // Base address ko 3 parts mein split karke store karo
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    // Limit ko split karke store karo
    gdt[num].limit_low   = (limit & 0xFFFF);

    // Granularity byte ke upper 4 bits + limit ke upper 4 bits
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);

    // Access byte directly set karo
    gdt[num].access      = access;
}

void gdt_install(void) {
    // GDT pointer setup - CPU ko batayega GDT kahan hai aur kitni badi
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base  = (uint32_t)&gdt;

    // Entry 0: Null descriptor - CPU REQUIREMENT hai, sab zero
    // Ye "invalid segment" ko represent karta hai
    gdt_set_gate(0, 0, 0, 0, 0);

    // Entry 1: Kernel Code Segment
    // base=0, limit=4GB (0xFFFFF * 4KB granularity)
    // access=0x9A -> Present, Ring 0, Code segment, Executable, Readable
    // gran=0xCF   -> 4KB granularity, 32-bit mode
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Entry 2: Kernel Data Segment
    // access=0x92 -> Present, Ring 0, Data segment, Writable
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Entry 3: User Code Segment (future use - Ring 3)
    // access=0xFA -> Present, Ring 3, Code segment, Executable, Readable
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // Entry 4: User Data Segment (future use - Ring 3)
    // access=0xF2 -> Present, Ring 3, Data segment, Writable
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // Ab Assembly function ko call karo jo actually
    // CPU ko naya GDT load karwayega aur segment registers reload karega
    gdt_flush((uint32_t)&gp);
}
