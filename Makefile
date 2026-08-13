# Makefile -- NOVA OS build system

TARGET  = i686-elf
CC      = $(TARGET)-gcc
AS      = nasm
LD      = $(TARGET)-gcc

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

BUILD_DIR = build
ISO_DIR   = iso

BOOT_OBJ   = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_BIN = $(BUILD_DIR)/nova.bin
ISO_FILE   = $(BUILD_DIR)/nova.iso

.PHONY: all iso run clean

all: $(KERNEL_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): boot/boot.s | $(BUILD_DIR)
	$(AS) -f elf32 boot/boot.s -o $(BOOT_OBJ)

$(KERNEL_OBJ): kernel/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/kernel.c -o $(KERNEL_OBJ)

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(LDFLAGS) $(BOOT_OBJ) $(KERNEL_OBJ)

iso: $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/nova.kernel
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO_FILE)

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)
