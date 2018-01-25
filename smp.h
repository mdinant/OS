#ifndef __SMP_H
#define __SMP_H

#include <system.h>
#include "idt.h"

//enum ProcessorState {
//	WAITING = 0, BUSY = 1
//};

#define WAITING		0
#define BUSY		1


typedef struct {
	unsigned char ApicId;
	bool isBsp;
	int state;
	int esp;
	//int stack;

	/* Declare an IDT of 256 entries. Although we will only use the
	*  first 32 entries in this tutorial, the rest exists as a bit
	*  of a trap. If any undefined IDT entry is hit, it normally
	*  will cause an "Unhandled Interrupt" exception. Any descriptor
	*  for which the 'presence' bit is cleared (0) will generate an
	*  "Unhandled Interrupt" exception */
	struct idt_entry idt[256];
	struct idt_ptr idtp;

	void *irq48_routine;

	/* Our GDT, with 3 entries, and finally our special GDT pointer */
	//struct gdt_entry gdt[3];
	//struct gdt_ptr gp;

	unsigned int irq_count;


} processor_t;


typedef struct {
	unsigned long IOApicAddress;
	unsigned int localApicAddress;
	processor_t processorList[MAX_CPU];
	int numberOfProcessors;
} smp_t;


void initSMP();

#endif
