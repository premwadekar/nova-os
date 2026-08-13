/* linker.ld -- memory layout for NOVA OS kernel */

ENTRY(_start)

SECTIONS
{
    /* Kernel starts at 1MB, standard Multiboot-safe address */
    . = 1M;

    /* Multiboot header must come first so GRUB finds it early */
    .multiboot ALIGN(4K) : {
        *(.multiboot)
    }

    /* Executable code */
    .text ALIGN(4K) : {
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN(4K) : {
        *(.rodata)
    }

    /* Initialized data */
    .data ALIGN(4K) : {
        *(.data)
    }

    /* Uninitialized data + stack */
    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }
}
