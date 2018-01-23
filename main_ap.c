#include <system.h>
#include "smp.h"
#include "apic.h"

extern smp_t smp;
extern int _running_flag;


void test_stack(int val) {
	//memcpy((char*)0xb8000, (char *)&val, 4);
	//char * c = (char*)0xb8000;
	//*c = (char*)(&val);
	//val = 666;

}

void _main_ap(int processorNum) {

	__asm__ __volatile__ ("sti");

	//int val = 0x2f4b2f4f;
	//test_stack(0x2f4b2f4f);

	/**
	 * AP Shopping list:
	 */
	idt_install(smp.processorList[processorNum].idt, &smp.processorList[processorNum].idtp);
	isrs_install(smp.processorList[processorNum].idt);
	apic_write(SPURIOUS_INTERRUPT_VECTOR_REGISTER, 0x1FF);
	apic_irq_install(smp.processorList[processorNum].idt);	// so we can receive
	_running_flag = 0;
//    settextcolor(10,16);
//	apic_write(INTERRUPT_COMMAND_REGISTER_2, smp.processorList[0].ApicId << 24);
//	apic_write(INTERRUPT_COMMAND_REGISTER_1, 0x4400);
//	apic_write(INTERRUPT_COMMAND_REGISTER_1, 0x4030);


	for (;;); // or halt
}
