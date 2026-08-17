; irq_stubs.s -- Hardware interrupt (IRQ) entry points (IRQ0-15, mapped to interrupts 32-47)
[bits 32]

section .text

extern irq_handler   ; C function jo hum call karenge (irq.c mein defined)

; Common stub -- saare IRQ yahan jump karte hain registers push karne ke baad
irq_common_stub:
    pusha                ; eax, ecx, edx, ebx, esp, ebp, esi, edi push karo

    mov ax, ds           ; current data segment save karo
    push eax

    mov ax, 0x10          ; kernel data segment load karo (GDT entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler       ; C handler ko call karo

    pop eax                ; purana data segment wapas restore karo
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                   ; saare general registers wapas restore karo
    add esp, 8              ; error code aur interrupt number stack se hatao
    iret                    ; interrupt se wapas jao

; Macro: IRQ stubs generate karta hai
; %1 = IRQ number (jaise 0-15), %2 = interrupt number (32-47)
%macro IRQ 2
global irq%1
irq%1:
    push dword 0        ; dummy error code (IRQs kabhi error code nahi dete)
    push dword %2         ; interrupt number (32-47 range)
    jmp irq_common_stub
%endmacro

IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
