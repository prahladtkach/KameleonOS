#include "vga.h"
#include "io.h"
#include "string.h"

static uint16_t *vga_buffer;
static int cursor_x;
static int cursor_y;
static uint8_t vga_attrib;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

void vga_enable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | 13);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

void vga_move_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_init(void) {
    vga_buffer = (uint16_t *)VGA_MEMORY;
    cursor_x = 0;
    cursor_y = 0;
    vga_attrib = vga_color(VGA_LIGHT_GREEN, VGA_BLACK);
    vga_clear();
    vga_enable_cursor();
}

void vga_clear(void) {
    uint16_t blank = vga_entry(' ', vga_attrib);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_move_cursor(cursor_x, cursor_y);
}

void vga_set_color(vga_color_t fg, vga_color_t bg) {
    vga_attrib = vga_color(fg, bg);
}

void vga_scroll(void) {
    uint16_t blank = vga_entry(' ', vga_attrib);

    if (cursor_y >= VGA_HEIGHT) {
        /* Move everything up one row */
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        /* Clear the last row */
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = blank;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~7;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, vga_attrib);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    vga_scroll();
    vga_move_cursor(cursor_x, cursor_y);
}

void vga_backspace(void) {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', vga_attrib);
    vga_move_cursor(cursor_x, cursor_y);
}

void vga_print(const char *str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_print_at(const char *str, int x, int y) {
    cursor_x = x;
    cursor_y = y;
    vga_print(str);
}

void vga_print_hex(uint32_t value) {
    const char hex[] = "0123456789ABCDEF";
    char buf[11];
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        buf[2 + (7 - i)] = hex[(value >> (i * 4)) & 0xF];
    }
    buf[10] = '\0';
    vga_print(buf);
}

void vga_print_dec(int value) {
    if (value == 0) {
        vga_putchar('0');
        return;
    }

    if (value < 0) {
        vga_putchar('-');
        value = -value;
    }

    char buf[12];
    int i = 0;
    while (value > 0) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (--i >= 0) {
        vga_putchar(buf[i]);
    }
}
