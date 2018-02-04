#include <system.h>
#include "vbe.h"

#include "apic.h"
#include "smp.h"
#include "render.h"

extern screen_t screen;


vbeControllerInfo * ctrl = (vbeControllerInfo *) VBE_INFO_ADDR;
vbeModeInfo * inf = (vbeModeInfo *) VBE_MODE_INFO_ADDR;

extern smp_t smp;
//extern int _ap_stack;

bool isModeInList(unsigned short modeNum, vbeControllerInfo * ctrl) {
	unsigned short * mode = (unsigned short*) segToFlatAddr(
			ctrl->VideoModesPtr);
	while (*mode != 0xFFFF) {
		if (*mode == modeNum) {
			return TRUE;
		}
		mode += 2;
	}
	return FALSE;
}

enum VESARet setVESAMode(unsigned short modeNum, bool flat) {
	regs32_t regs;
	regs.eax = COMMAND_SET_SUPERVGA_MODE;
	regs.ebx = modeNum;

	if (flat) {
		regs.ebx |= 1 << 14;
	}

	return VESAFunc(&regs);
}

enum VESARet VESAFunc(regs32_t * regs) {
	int32_beta(0x10, regs);

	if (regs->eax != 0x4F) {
		return FunctionNotSupported;
	}

	return FunctionCallSuccessful;
}

enum VESARet getSuperVGAInformation(vbeControllerInfo * ctrl) {
	regs32_t regs;

	memcpy(ctrl, "VBE2", 4);
	memset(&regs, 0, sizeof(regs32_t));

	regs.eax = COMMAND_GET_SUPERVGA_INFORMATION;
	regs.edi = VBE_INFO_ADDR;
	regs.es = 0;

	return VESAFunc(&regs);

}
enum VESARet getSuperVGAModeInformation(unsigned short modeNum) {
	regs32_t regs;
	memset(&regs, 0, sizeof(regs32_t));

	regs.eax = COMMAND_GET_SUPERVGA_MODE_INFORMATION;
	regs.ecx = modeNum;
	regs.edi = VBE_MODE_INFO_ADDR;
	regs.es = 0;

	return VESAFunc(&regs);

}
void printModesWithBBP(unsigned short bbp, vbeControllerInfo * ctrl) {
	printf("modes with bbp %u:\n", bbp);
	unsigned short * mode = (unsigned short*) segToFlatAddr(
			ctrl->VideoModesPtr);
	while (*mode != 0xFFFF) {
		char hex[8];
		hex_to_char(*mode, hex);
		enum VESARet ret = getSuperVGAModeInformation(*mode);

		switch (ret) {
		case FunctionNotSupported: {
			printf("Function Not Supported for mode: %s\n", hex);
			break;
		}
		case FunctionCallFailed: {
			printf("Function Call Failed for mode: %s\n", hex);
			break;
		}
		default: {
			// do something
			if (inf->BitsPerPixel == bbp) {
				hex_to_char(*mode, hex);
				printf("%4s  ", hex);

				if (CHECK_BIT(inf->ModeAttributes, 0) == 0) {
					printf("- NOT SUPPORTED IN HARWARE - ");
				}
				printf("%4u %4u  ", inf->XResolution, inf->YResolution);
				if (CHECK_BIT(inf->ModeAttributes, 7)) {
					printf("%3s ", "lfb");
				}
				if (CHECK_BIT(inf->ModeAttributes, 6)) {
					printf("%4s\t", "only");
				} else {
					printf("%3s\t", "win");
				}
				printf("BPS: %7u\t", inf->BytesPerScanLine);
				printf("NoIP: %2u\t", inf->NumberOfImagePages);
//				hex_to_char(inf->PhysBasePtr, hex);
//				printf("physPtr: 0x%8s", hex);

				printf("\n");
			}
			break;
		}
		}
		mode += 2;
	}
}

void printVbeControllerInfo(vbeControllerInfo * ctrl) {
	char hex[8];
	printf("VESASignature: %s\n", ctrl->VESASignature);
	printf("VESAVersion: %u (major), %u (minor)\n", ctrl->VESAVersion >> 8,
			ctrl->VESAVersion & 0xFF);
	printf("OEMString: %s\n", (char*) segToFlatAddr(ctrl->OEMStringPtr));

	if (CHECK_BIT(ctrl->Capabilities[0], 7)) {

		printf("Capabilities: DAC width is switchable\n");
	} else {
		printf(
				"Capabilities: DAC is fixed width, with 6-bits per primary color\n");

	}
	//printf("capability: %u\n", ctrl->Capabilities[0]);

//			if (((ctrl->Capabilities[0] >> 0) & 0x01) == 1) {
//				printf(
//						"Capabilities: DAC is fixed width, with 6-bits per primary color\n");
//			} else
//				printf("Capabilities: DAC width is switchable\n");

	printf("Video Modes Available:\n");
	unsigned short * mode = (unsigned short*) segToFlatAddr(
			ctrl->VideoModesPtr);
	while (*mode != 0xFFFF) {
		hex_to_char(*mode, hex);
		printf(" | 0x%s", hex);
		mode += 2;
	}
	printf("\n");
	printf("TotalMemory (dec): %u\n", ((long) (ctrl->TotalMemory)) * 0xFFFF);

}

bool showVesaInfo() {
//	long long tlonglong;
//	printf("sizeof long long: %u\n", sizeof(tlonglong));
//	printf("GET_SUPERVGA_INFORMATION... ");
//	anykey();
	enum VESARet ret = getSuperVGAInformation(ctrl);

	switch (ret) {
	case FunctionNotSupported: {
		printf("Function Not Supported\n");
		return FALSE;
	}
	case FunctionCallFailed: {
		printf("Function Call Failed\n");
		return FALSE;
	}
	default: {
		printf("FunctionCallSuccessful\n");
	}
	}

	printVbeControllerInfo(ctrl);
//    unsigned short modeNum = 0x178;
//    if (isModeInList(modeNum, ctrl)) {
//    	printf("ModeNum 0x178 is in list");
//    } else {
//    	printf("ModeNum 0x178 is NOT in list");
//    }
	printf("Press ENTER for list modes with bbp 15");
	anykey();
	printModesWithBBP(15, ctrl);
	printf("Press ENTER for list modes with bbp 16");
	anykey();
	printModesWithBBP(16, ctrl);
	printf("Press ENTER for list modes with bbp 24");
	anykey();
	printModesWithBBP(24, ctrl);
	printf("Press ENTER for list modes with bbp 32");
	anykey();
	printModesWithBBP(32, ctrl);

	return TRUE;

}

bool tryMode(unsigned short modeNum) {

	char hex[8];
	hex_to_char(modeNum, hex);

	enum VESARet ret;
	ret = getSuperVGAModeInformation(modeNum);
	switch (ret) {
	case FunctionNotSupported: {
		printf("getSuperVGAModeInformation Not Supported for mode: %s\n", hex);
		return FALSE;
	}
	case FunctionCallFailed: {
		printf("getSuperVGAModeInformation Failed for mode: %s\n", hex);
		return FALSE;
	}
	default:
		hex_to_char(inf->PhysBasePtr, hex);
		printf("press ENTER to set mode %4u %4u %2u lfb at %8s...",
				inf->XResolution, inf->YResolution, inf->BitsPerPixel, hex);
		anykey();
		break;
	}

	ret = setVESAMode(modeNum, TRUE);

	switch (ret) {
	case FunctionNotSupported: {
		printf("SetVESAMode Not Supported for mode: %s\n", hex);
		return FALSE;
	}
	case FunctionCallFailed: {
		printf("SetVESAMode Failed for mode: %s\n", hex);
		return FALSE;
	}
	default: {
		// now in video mode :)
		//	anykey();



		break;
	}


	}




	return TRUE;
}

bool init_vbe() {

	//return showVesaInfo();
return TRUE;
}

void apic_irq48_handler(struct regs *r) {
	//int val = 0x2f4b2f4f;
	//memcpy((char*)0xb8000, (char *)&val, 4);
	//register int id asm("ebp");
//cls();
	//smp.processorList[1].state = 1;
	//smp.processorList[id].irq_count++;


}


void demoVBE() {

	bool result = tryMode(0x145);

	if (result == FALSE) {
		setVESAMode(3, FALSE);
		printf("Could not set mode\n");
		return;
	}




	screen.bufSize = inf->BytesPerScanLine * inf->YResolution;
	screen.bBuffer = (char *) (inf->PhysBasePtr + screen.bufSize);
	screen.bbp = inf->BitsPerPixel * 8;
	screen.width = inf->XResolution;
	screen.height = inf->YResolution;

	size_t size = screen.bufSize / 4;

	char * start = &screen.bBuffer[0 * size];
	char * end = &screen.bBuffer[(0 * size) + size];

//	renderScreenBufferPart(&screen.bBuffer[processorNum * size], &screen.bBuffer[size], 200);
	char blue[] = {255, 0, 0, 0};
	char green[] = {0, 255, 0, 0};
	char red[] = {0, 0, 255, 0};
	char white[] = {255, 255, 255, 0};
	char * color;
	switch (0) {
	case 0:
		color = white;
		break;
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

	int i;
	for (i = 1; i < smp.numberOfProcessors; i++) {
		smp.processorList[i].state = BUSY;
	}



	bool done = FALSE;


	while (done == FALSE) {
		done = TRUE;
		for (i = 1; i < smp.numberOfProcessors; i++) {
			acquireLock(&smp.processorList[i].lState);
			if (smp.processorList[i].state == BUSY) {
				done = FALSE;
				releaseLock(&smp.processorList[i].lState);
				break;
			}
			releaseLock(&smp.processorList[i].lState);
		}
	}

	char * ptr = (char *) inf->PhysBasePtr;
	memcpy(ptr, screen.bBuffer, screen.bufSize);


	// do a color



	//memcpy_SSE2(ptr, bBuffer, bufSize);

	//memset(ptr, 128, 2000);

	anykey();


	setVESAMode(3, FALSE);

	printf("state1: %u\n", smp.processorList[1].state);
	printf("state2: %u\n", smp.processorList[2].state);
	printf("state3: %u\n", smp.processorList[3].state);
}
