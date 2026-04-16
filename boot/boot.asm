; Kameleon OS - Multiboot compliant boot entry
; Sets up stack and jumps to C kernel

MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1
MBOOT_MAGIC         equ 0x1BADB002
MBOOT_FLAGS         equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384          ; 16 KB stack
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top  ; set up the stack
    push ebx            ; push multiboot info pointer
    push eax            ; push multiboot magic number
    call kernel_main    ; call C kernel
    cli                 ; disable interrupts
.hang:
    hlt                 ; halt the CPU
    jmp .hang           ; loop forever if we somehow wake up
