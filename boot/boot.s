; boot.s -- NOVA OS entry point + Multiboot header

; --- Multiboot header constants ---
MBALIGN  equ  1<<0              ; align loaded modules on page boundaries
MEMINFO  equ  1<<1              ; provide memory map to kernel
FLAGS    equ  MBALIGN | MEMINFO ; multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; magic number GRUB looks for
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum must make (MAGIC+FLAGS+CHECKSUM)=0

; --- Multiboot header section ---
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; --- Reserve space for our stack ---
section .bss
align 16
stack_bottom:
    resb 16384      ; 16 KiB stack
stack_top:

; --- Entry point ---
section .text
global _start
_start:
    mov esp, stack_top   ; set up the stack pointer

    extern kernel_main
    call kernel_main      ; call our C kernel

    cli                   ; clear interrupts (disable them)
.hang:
    hlt                   ; halt the CPU
    jmp .hang             ; if it ever wakes up, halt again
