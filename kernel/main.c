#include "types.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main(uint32_t magic, uint32_t mboot_addr) {
    (void)mboot_addr;

    /* Initialize VGA text mode */
    vga_init();

    /* Verify multiboot */
    if (magic != 0x2BADB002) {
        vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
        vga_print("ERROR: Not booted by a multiboot-compliant bootloader!\n");
        return;
    }

    /* Initialize Global Descriptor Table */
    gdt_init();

    /* Initialize Interrupt Descriptor Table and PIC */
    idt_init();

    /* Initialize keyboard driver */
    keyboard_init();

    /* Initialize and start the shell */
    shell_init();

    /* Set keyboard callback to shell input handler */
    keyboard_set_callback(shell_handle_key);

    /* Enable interrupts and halt until the next one fires */
    __asm__ volatile ("sti");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}
