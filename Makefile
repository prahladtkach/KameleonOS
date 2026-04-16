# Kameleon OS - Build System
#
# Prerequisites (install via your package manager):
#   - i686-elf cross compiler (i686-elf-gcc, i686-elf-ld)
#   - nasm (assembler)
#   - grub-mkrescue + xorriso (for ISO creation)
#   - qemu-system-i386 (for testing)

CC      = i686-elf-gcc
LD      = i686-elf-ld
AS      = nasm
CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude -nostdlib -nostdinc
LDFLAGS = -T linker.ld -nostdlib

# Source files
ASM_SOURCES = boot/boot.asm boot/gdt.asm boot/interrupt.asm
C_SOURCES   = kernel/main.c kernel/gdt.c kernel/idt.c kernel/shell.c \
              drivers/vga.c drivers/keyboard.c lib/string.c

# Object files
ASM_OBJECTS = $(ASM_SOURCES:.asm=.o)
C_OBJECTS   = $(C_SOURCES:.c=.o)
OBJECTS     = $(ASM_OBJECTS) $(C_OBJECTS)

# Output
KERNEL  = kameleon.bin
ISO     = kameleon.iso
ISO_DIR = isodir

.PHONY: all clean run iso

all: $(KERNEL)

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.asm
	$(AS) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/$(KERNEL)
	echo 'set timeout=0'                         >  $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0'                         >> $(ISO_DIR)/boot/grub/grub.cfg
	echo ''                                      >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "Kameleon OS" {'             >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    multiboot /boot/$(KERNEL)'         >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    boot'                              >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                     >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO)

run-kernel: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)
