# Makefile -- NOVA OS build system

TARGET  = i686-elf
CC      = $(TARGET)-gcc
AS      = nasm
LD      = $(TARGET)-gcc

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

BUILD_DIR = build
ISO_DIR   = iso

BOOT_OBJ        = $(BUILD_DIR)/boot.o
KERNEL_OBJ      = $(BUILD_DIR)/kernel.o
GDT_OBJ         = $(BUILD_DIR)/gdt.o
GDT_FLUSH_OBJ   = $(BUILD_DIR)/gdt_flush.o
IDT_OBJ         = $(BUILD_DIR)/idt.o
IDT_FLUSH_OBJ   = $(BUILD_DIR)/idt_flush.o
ISR_OBJ         = $(BUILD_DIR)/isr.o
ISR_STUBS_OBJ   = $(BUILD_DIR)/isr_stubs.o
IRQ_OBJ         = $(BUILD_DIR)/irq.o
IRQ_STUBS_OBJ   = $(BUILD_DIR)/irq_stubs.o
KEYBOARD_OBJ    = $(BUILD_DIR)/keyboard.o
KERNEL_BIN      = $(BUILD_DIR)/nova.bin
ISO_FILE        = $(BUILD_DIR)/nova.iso

.PHONY: all iso run clean

all: $(KERNEL_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): boot/boot.s | $(BUILD_DIR)
	$(AS) -f elf32 boot/boot.s -o $(BOOT_OBJ)

$(KERNEL_OBJ): kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/kernel.c -o $(KERNEL_OBJ)

$(GDT_OBJ): kernel/arch/gdt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/arch/gdt.c -o $(GDT_OBJ)

$(GDT_FLUSH_OBJ): kernel/arch/gdt_flush.s | $(BUILD_DIR)
	$(AS) -f elf32 kernel/arch/gdt_flush.s -o $(GDT_FLUSH_OBJ)

$(IDT_OBJ): kernel/arch/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/arch/idt.c -o $(IDT_OBJ)

$(IDT_FLUSH_OBJ): kernel/arch/idt_flush.s | $(BUILD_DIR)
	$(AS) -f elf32 kernel/arch/idt_flush.s -o $(IDT_FLUSH_OBJ)

$(ISR_OBJ): kernel/arch/isr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/arch/isr.c -o $(ISR_OBJ)

$(ISR_STUBS_OBJ): kernel/arch/isr_stubs.s | $(BUILD_DIR)
	$(AS) -f elf32 kernel/arch/isr_stubs.s -o $(ISR_STUBS_OBJ)

$(IRQ_OBJ): kernel/arch/irq.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/arch/irq.c -o $(IRQ_OBJ)

$(IRQ_STUBS_OBJ): kernel/arch/irq_stubs.s | $(BUILD_DIR)
	$(AS) -f elf32 kernel/arch/irq_stubs.s -o $(IRQ_STUBS_OBJ)

$(KEYBOARD_OBJ): kernel/drivers/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/drivers/keyboard.c -o $(KEYBOARD_OBJ)

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(GDT_OBJ) $(GDT_FLUSH_OBJ) $(IDT_OBJ) $(IDT_FLUSH_OBJ) $(ISR_OBJ) $(ISR_STUBS_OBJ) $(IRQ_OBJ) $(IRQ_STUBS_OBJ) $(KEYBOARD_OBJ) linker.ld
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(LDFLAGS) $(BOOT_OBJ) $(KERNEL_OBJ) $(GDT_OBJ) $(GDT_FLUSH_OBJ) $(IDT_OBJ) $(IDT_FLUSH_OBJ) $(ISR_OBJ) $(ISR_STUBS_OBJ) $(IRQ_OBJ) $(IRQ_STUBS_OBJ) $(KEYBOARD_OBJ)

iso: $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/nova.kernel
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO_FILE)

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)
