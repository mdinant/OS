#include <system.h>
#include "smp.h"
#include "apic.h"

extern smp_t smp;
extern int _running_flag;


extern int _get_esp();
extern int _ap_stack;
void test_func_c() {
	memcpy((char*)0xb8000, (char *)0xb8000, 4);
}

void test_stack(int val) {
	memcpy((char*)0xb8000, (char *)&val, 4);

	//char * c = (char*)0xb8000;
	//*c = (char*)(&val);
	//val = 666;

}



void _main_ap(int processorNum) {
	int esp = _get_esp();
	__asm__ __volatile__ ("sti");

	//int val = 0x2f4b2f4f;
	//test_stack(0x2f4b2f4f);

	/**
	 * AP Shopping list:
	 */
	idt_install(smp.processorList[processorNum].idt, &smp.processorList[processorNum].idtp);
	isrs_install(smp.processorList[processorNum].idt);

	//apic_irq_install(smp.processorList[processorNum].idt);	// so we can receive
	//apic_write(SPURIOUS_INTERRUPT_VECTOR_REGISTER, 0x1FF);
	smp.processorList[processorNum].esp = esp;
	//smp.processorList[processorNum].state = processorNum;
	//int temp = _running_flag;
	_running_flag = 0;	// RESET flag to inform bsp we are up and running



	//cls();

	//processor_t * processor = &smp.processorList[processorNum];

	//while (smp.processorList[processorNum].state == 0) {}
	//smp.processorList[processorNum].irq_count++;
//	void (*functionPtr)(int) = &test_stack;
//
//	(*functionPtr)(0x2f4b2f4f);
	if (processorNum == 1) {
		//cls();
		while(smp.processorList[1].state == 0) {
			//printf("state is still 0\n");
			//cls();
		}
		//cls();
		smp.processorList[1].irq_count = 666;
	}

	while (TRUE){}
}
