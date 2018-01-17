#include <system.h>
#include "smp.h"
#include "apic.h"

extern smp_t smp;

void test_stack(int val) {
	memcpy((char*)0xb8000, (char *)&val, 4);
}

void _main_ap()
{
	__asm__ __volatile__ ("sti");

	//int val = 0x2f4b2f4f;
	test_stack(0x2f4b2f4f);


	apic_write(INTERRUPT_COMMAND_REGISTER_2, smp.processorList[0].ApicId << 24);
//	apic_write(INTERRUPT_COMMAND_REGISTER_1, 0x4400);
	apic_write(INTERRUPT_COMMAND_REGISTER_1, 0x4030);


	for (;;); // or halt
}
