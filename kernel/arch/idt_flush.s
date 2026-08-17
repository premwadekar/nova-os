; idt_flush.s -- CPU ko naya IDT load karta hai
[bits 32]

section .text
global idt_flush

; idt_flush(uint32_t idt_ptr_address)
idt_flush:
    mov eax, [esp+4]   ; argument uthao stack se -- IDT pointer ka address
    lidt [eax]         ; CPU ko naya IDT load karo -- ye main instruction hai
    ret
