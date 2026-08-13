[bits 32]

section .text
global gdt_flush

; gdt_flush(uint32_t gdt_ptr_address)
; Stack pe ek hi argument hai - GDT pointer ka address
gdt_flush:
    mov eax, [esp+4]   ; Argument uthao stack se (gp ka address)
    lgdt [eax]          ; CPU ko naya GDT load karo - ye main step hai!

    ; Data segment registers ko update karo
    ; 0x10 = offset of Kernel Data Segment (entry 2 * 8 bytes)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Code segment (CS) update karne ke liye far jump zaroori hai
    ; 0x08 = offset of Kernel Code Segment (entry 1 * 8 bytes)
    jmp 0x08:.flush_done

.flush_done:
    ret
