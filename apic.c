//#include <system.h>
#include "apic.h"
#include "smp.h"
//unsigned long localApicAddress;

extern smp_t smp;


unsigned int apic_read(unsigned int reg) {

	return *((unsigned int*)(smp.localApicAddress + reg));
}

void apic_write(unsigned int reg, unsigned int value) {
	*((unsigned int*)(smp.localApicAddress + reg)) = value;
}

unsigned int apic_wait_icr_idle(void) {
	unsigned int send_status;
	int timeout;

	timeout = 0;
	do {
		send_status = apic_read(INTERRUPT_COMMAND_REGISTER_1) & 0x01000;

		if (!send_status)
			break;
		sleep(10);
	} while (timeout++ < 100);

	return send_status;
}
