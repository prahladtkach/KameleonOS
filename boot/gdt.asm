; Kameleon OS - GDT flush routine
; Called from C to reload segment registers after GDT is set up

global gdt_flush
extern gdt_ptr

gdt_flush:
    lgdt [gdt_ptr]      ; load the GDT
    mov ax, 0x10         ; 0x10 = offset to data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush      ; far jump to code segment
.flush:
    ret

global idt_flush
idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret
