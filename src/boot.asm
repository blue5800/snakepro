section .boot
[bits 16]
extern SECTORS_TO_LOAD
global _start
_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00


    mov [BOOT_DRIVE], dl 

    mov bx, 0x7e00
    mov dh, SECTORS_TO_LOAD 
    mov dl, [BOOT_DRIVE]
    call disk_load

    extern _stage2_start
    jmp _stage2_start

disk_load:
    PUSH DX 
    MOV AH, 0X02
    MOV AL, DH
    MOV CH, 0X00
    MOV DH, 0X00
    MOV CL, 0X02        ; START READING FROM SECOND SECTOR (I.E. AFTER THE BOOT SECTOR)
    INT 0X13            ; BIOS INTERRUPT

    JC disk_error   

    POP DX
    CMP DH, AL
    JNE disk_error
    RET

disk_error:
    jmp $               ; how did you even get here?

BOOT_DRIVE: db 0




times 510-($-$$) db 0   ; pad rest of sector
dw 0xAA55               ; Boot sector signature
