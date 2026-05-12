#include <stdint.h>
struct idt_entry {
    uint16_t offset_low;   // Lower 16 bits of handler function address
    uint16_t selector;     // Code segment selector in GDT
    uint8_t zero;         // Unused, set to 0
    uint8_t type_attr;    // Type and attributes
    uint16_t offset_high;  // Higher 16 bits of handler function address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;       // Size of the IDT
    uint32_t base;        // Base address of the IDT
} __attribute__((packed));

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();
