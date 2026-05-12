/*
* idt.c - interrupt descriptor table (IDT) management
*/
#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA    0x21
#define SLAVE_PIC_COMMAND  0xA0
#define SLAVE_PIC_DATA     0xA1

#include <idt.h>


static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait() {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    io_wait();
}



static void PIC_remap(int offset1, int offset2) {
    // save the current masks
    uint8_t a1 = inb(MASTER_PIC_DATA);
    uint8_t a2 = inb(SLAVE_PIC_DATA);

    // start the initialization sequence (in cascade mode)
    outb(MASTER_PIC_COMMAND, 0x11);
    outb(SLAVE_PIC_COMMAND, 0x11);

    // set the new offsets for the PICs
    outb(MASTER_PIC_DATA, offset1); // Master PIC vector offset
    outb(SLAVE_PIC_DATA, offset2);  // Slave PIC vector offset

    // tell Master PIC about the presence of slave PIC at IRQ2 (0000 0100)
    outb(MASTER_PIC_DATA, 0x04);
    // tell slave PIC its cascade identity (0000 0010)
    outb(SLAVE_PIC_DATA, 0x02);
    // set the PICs to operate in 8086/88 mode
    outb(MASTER_PIC_DATA, 0x01);
    outb(SLAVE_PIC_DATA, 0x01);

    // restore saved masks
    outb(MASTER_PIC_DATA, a1);
    outb(SLAVE_PIC_DATA, a2);
}

extern void isr0(); // declare the ISR handler for interrupt 0

__attribute__((aligned(0x10))) static struct idt_entry idt[256];
struct idt_ptr idt_ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;       // lower 16 bits of handler address
    idt[num].selector = sel;                    // code segment selector
    idt[num].zero = 0;                         // Unused
    idt[num].type_attr = flags;                // type and attributes
    idt[num].offset_high = (base >> 16) & 0xFFFF; // high 16 bits of handler address
}

void idt_install(){
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt;

    //zero the IDT to avoid garbage values
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }
    
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);

    PIC_remap(0x20, 0x28);

    __asm__ volatile ("lidt (%0)" : : "r" (&idt_ptr));
}


