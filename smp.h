#ifndef __SMP_H
#define __SMP_H

#include <system.h>
#include "idt.h"

typedef struct {
	unsigned char ApicId;
	bool isBsp;
	/* Declare an IDT of 256 entries. Although we will only use the
	*  first 32 entries in this tutorial, the rest exists as a bit
	*  of a trap. If any undefined IDT entry is hit, it normally
	*  will cause an "Unhandled Interrupt" exception. Any descriptor
	*  for which the 'presence' bit is cleared (0) will generate an
	*  "Unhandled Interrupt" exception */
	struct idt_entry idt[256];
	struct idt_ptr idtp;
} processor_t;


typedef struct {
	unsigned long IOApicAddress;
	unsigned int localApicAddress;
	processor_t processorList[MAX_CPU];
	int numberOfProcessors;
} smp_t;


void initSMP();

#endif
