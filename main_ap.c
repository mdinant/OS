#include <system.h>
#include "smp.h"
#include "apic.h"

#include "render.h"

extern smp_t smp;
extern int _running_flag;
extern screen_t screen;


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

	__asm__ __volatile__ ("sti");

	/**
	 * AP Shopping list:
	 */
	idt_install(smp.processorList[processorNum].idt, &smp.processorList[processorNum].idtp);
	isrs_install(smp.processorList[processorNum].idt);

	//apic_irq_install(smp.processorList[processorNum].idt);	// so we can receive
	//apic_write(SPURIOUS_INTERRUPT_VECTOR_REGISTER, 0x1FF);

	smp.processorList[processorNum].state = WAITING;
	smp.processorList[processorNum].stateLock.item = (int*)&smp.processorList[processorNum].state;
	_running_flag = 0;	// RESET flag to inform bsp we are up and running

	while (smp.processorList[processorNum].state == WAITING) {
		// do some NOP or something
	}

	//renderScreenPart(0, 0, 100, 100);

	//size_t size = screen.bufSize / smp.numberOfProcessors;
	size_t size = screen.bufSize / 4;

	char * start = &screen.bBuffer[processorNum * size];
	char * end = &screen.bBuffer[(processorNum * size) + size];

//	renderScreenBufferPart(&screen.bBuffer[processorNum * size], &screen.bBuffer[size], 200);
	char blue[] = {255, 0, 0, 0};
	char green[] = {0, 255, 0, 0};
	char red[] = {0, 0, 255, 0};
	char * color;
	switch (processorNum) {
	case 1:
		color = red;
		break;
	case 2:
		color = blue;
		break;
	case 3:
		color = green;
		break;
	default:
		break;
	}

	renderScreenBufferPart(start, end, color);

	//char red[] = RED;
	while (smp.processorList[processorNum].state == BUSY) {
		smp.processorList[processorNum].state = WAITING;
	}
}
