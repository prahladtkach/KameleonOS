#include "gdt.h"

#define GDT_ENTRIES 5

static gdt_entry_t gdt[GDT_ENTRIES];
gdt_ptr_t gdt_ptr;

extern void gdt_flush(void);

static void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].base_low    = (base & 0xFFFF);
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;
    gdt[i].limit_low   = (limit & 0xFFFF);
    gdt[i].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].access      = access;
}

void gdt_init(void) {
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    gdt_set_entry(0, 0, 0,          0,    0);    /* Null segment */
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Kernel code segment */
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Kernel data segment */
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User code segment */
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User data segment */

    gdt_flush();
}
