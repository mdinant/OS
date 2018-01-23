#ifndef __IRQ_H
#define __IRQ_H



void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install(struct idt_entry * idt);
void irq_remap(void);



#endif
