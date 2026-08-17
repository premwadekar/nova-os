; isr_stubs.s -- CPU exception entry points (interrupts 0-31)
[bits 32]

section .text

extern isr_handler   ; C function jo hum call karenge

; Common stub -- saare ISR yahan jump karte hain registers push karne ke baad
isr_common_stub:
    pusha                ; eax, ecx, edx, ebx, esp, ebp, esi, edi push karo

    mov ax, ds           ; current data segment save karo
    push eax

    mov ax, 0x10          ; kernel data segment load karo (GDT entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler      ; C handler ko call karo -- esp registers_t* ki tarah pass hota hai

    pop eax               ; purana data segment wapas restore karo
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                  ; saare general registers wapas restore karo
    add esp, 8             ; error code aur interrupt number stack se hatao
    iret                   ; interrupt se wapas jao (CS, EIP, EFLAGS restore karta hai)

; Macro: exceptions jo error code NAHI dete -- humein dummy 0 push karna padta hai
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push dword 0        ; dummy error code
    push dword %1        ; interrupt number
    jmp isr_common_stub
%endmacro

; Macro: exceptions jo CPU khud error code push karta hai
%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push dword %1        ; interrupt number (error code CPU ne already push kar diya)
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0    ; Division By Zero
ISR_NOERRCODE 1    ; Debug
ISR_NOERRCODE 2    ; Non Maskable Interrupt
ISR_NOERRCODE 3    ; Breakpoint
ISR_NOERRCODE 4    ; Overflow
ISR_NOERRCODE 5    ; Out of Bounds
ISR_NOERRCODE 6    ; Invalid Opcode
ISR_NOERRCODE 7    ; No Coprocessor
ISR_ERRCODE   8    ; Double Fault
ISR_NOERRCODE 9    ; Coprocessor Segment Overrun
ISR_ERRCODE   10   ; Bad TSS
ISR_ERRCODE   11   ; Segment Not Present
ISR_ERRCODE   12   ; Stack Fault
ISR_ERRCODE   13   ; General Protection Fault
ISR_ERRCODE   14   ; Page Fault
ISR_NOERRCODE 15   ; Unknown Interrupt
ISR_NOERRCODE 16   ; Coprocessor Fault
ISR_ERRCODE   17   ; Alignment Check
ISR_NOERRCODE 18   ; Machine Check
ISR_NOERRCODE 19   ; Reserved
ISR_NOERRCODE 20   ; Reserved
ISR_NOERRCODE 21   ; Reserved
ISR_NOERRCODE 22   ; Reserved
ISR_NOERRCODE 23   ; Reserved
ISR_NOERRCODE 24   ; Reserved
ISR_NOERRCODE 25   ; Reserved
ISR_NOERRCODE 26   ; Reserved
ISR_NOERRCODE 27   ; Reserved
ISR_NOERRCODE 28   ; Reserved
ISR_NOERRCODE 29   ; Reserved
ISR_NOERRCODE 30   ; Reserved
ISR_NOERRCODE 31   ; Reserved
