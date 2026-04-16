#include "keyboard.h"
#include "idt.h"
#include "io.h"
#include "vga.h"

static keyboard_callback_t key_callback = NULL;

/* US keyboard scancode -> ASCII (lowercase) */
static const char scancode_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

/* US keyboard scancode -> ASCII (uppercase/shifted) */
static const char scancode_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static int shift_pressed = 0;

static void keyboard_handler(registers_t *regs) {
    (void)regs;
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    /* Track shift key state */
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    /* Ignore key releases (high bit set) */
    if (scancode & 0x80) {
        return;
    }

    if (scancode < sizeof(scancode_ascii)) {
        char c;
        if (shift_pressed) {
            c = scancode_shift[scancode];
        } else {
            c = scancode_ascii[scancode];
        }

        if (c && key_callback) {
            key_callback(c);
        }
    }
}

void keyboard_set_callback(keyboard_callback_t cb) {
    key_callback = cb;
}

void keyboard_init(void) {
    isr_register_handler(33, keyboard_handler); /* IRQ1 = INT 33 */
}
