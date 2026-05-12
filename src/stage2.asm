[bits 16]
extern kernel_main
global _stage2_start

_stage2_start:
    mov ah, 00h 
    mov al, 03h 
    int 10h        ; clr screen because honestly it's ugly when it tells loading floppy

    cli
    xor ax,ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 10h
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000    ; Set stack up somewhere safe (?)

    jmp kernel_main
    ;not reachable
    hlt

; ------------------------------------------------------------------
; GLOBAL DESCRIPTOR TABLE (GDT)
; ------------------------------------------------------------------

gdt_start:

    ; 1. Null Descriptor (Required by CPU)
    ; 8 bytes of zeros
    dq 0x0 

gdt_code:
    ; 2. Code Segment Descriptor
    dw 0xFFFF          ; Limit Low
    dw 0x0000          ; Base Low
    db 0x00            ; Base Middle
    db 10011010b       ; Access Byte: Present, Ring 0, Code Segment, Executable, Readable
    db 11001111b       ; Granularity: 4K granularity, 32-bit protected mode
    db 0x00            ; Base High

gdt_data:
    ; 3. Data Segment Descriptor
    dw 0xFFFF          ; Limit Low
    dw 0x0000          ; Base Low
    db 0x00            ; Base Middle
    db 10010010b       ; Access Byte: Present, Ring 0, Data Segment, Writable
    db 11001111b       ; Granularity: 4K granularity, 32-bit protected mode
    db 0x00            ; Base High

gdt_end:

; ------------------------------------------------------------------
; GDT DESCRIPTOR (This is what you load with LGDT)
; ------------------------------------------------------------------

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size (Limit): Always Size - 1
    dd gdt_start                ; Offset: Address of the table
