#ifndef __IDT_H
#define __IDT_H

/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void idt_set_gate(struct idt_entry * idt, unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install(struct idt_entry * idt, struct idt_ptr * idtp);

#endif
