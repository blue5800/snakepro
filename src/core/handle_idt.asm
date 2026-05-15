[extern handle_interrupt]

%macro ISR_NOERR 1
    global isr%1
    isr%1:
        push byte 0
        push %1
        jmp do_isr_common
%endmacro

%macro ISR_ERR 1
    global isr%1
    isr%1:
        push %1
        jmp do_isr_common
%endmacro

;--- ISRs for CPU exceptions (0-31)
ISR_NOERR 0 ; Divide by Zero
ISR_NOERR 1 ; Debug

ISR_ERR 8 ; Double Fault
ISR_ERR 13 ; General Protection Fault
ISR_NOERR 32 ; Timer
;--- enough of this

do_isr_common:
    ; Save registers
    pushad

    mov ax, ds 
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ;push and pop the stack pointer so the interrupt handler can actually use it (duh)
    push esp
    call handle_interrupt
    add esp, 4
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad
    add esp, 8 ; Clean up error code and interrupt number
    iret
