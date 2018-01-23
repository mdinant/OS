#ifndef __VBE_H
#define __VBE_H

#include <system.h>
#define VBE_FAR(x) 								unsigned short x[2]

// lekker bij elkaar
#define VBE_INFO_ADDR							0x2000
#define VBE_MODE_INFO_ADDR						0x3000
#define REALPTR									(unsigned int)
#define COMMAND_GET_SUPERVGA_INFORMATION    	0x4F00
#define COMMAND_GET_SUPERVGA_MODE_INFORMATION	0x4F01
#define COMMAND_SET_SUPERVGA_MODE				0x4F02

enum VESARet {
	FunctionSupported,
	FunctionNotSupported,
	FunctionCallSuccessful,
	FunctionCallFailed
};


typedef struct {
	unsigned char VESASignature[4];
	unsigned short VESAVersion;
	unsigned long OEMStringPtr;
	unsigned char Capabilities[4];
	unsigned long VideoModesPtr;
	unsigned short TotalMemory;
	unsigned short OemSoftwareRev;
	unsigned long OemVendorNamePtr;
	unsigned long OemProductNamePtr;
	unsigned long OemProductRevPtr;
	unsigned char Reserved[222];
	unsigned char OemData[256];
}__attribute__ ((packed)) vbeControllerInfo;

typedef struct {
	unsigned short ModeAttributes;
	unsigned char WinAAttributes;
	unsigned char WinBAttributes;
	unsigned short WinGranularity;
	unsigned short WinSize;
	unsigned short WinASegment;
	unsigned short WinBSegment;
	unsigned long WinFuncPtr;
	unsigned short BytesPerScanLine;
	unsigned short XResolution;
	unsigned short YResolution;
	unsigned char XCharSize;
	unsigned char YCharSize;
	unsigned char NumberOfPlanes;
	unsigned char BitsPerPixel;
	unsigned char NumberOfBanks;
	unsigned char MemoryModel;
	unsigned char BankSize;
	unsigned char NumberOfImagePages;
	unsigned char Reserved_page;
	unsigned char RedMaskSize;
	unsigned char RedMaskPos;
	unsigned char GreenMaskSize;
	unsigned char GreenMaskPos;
	unsigned char BlueMaskSize;
	unsigned char BlueMaskPos;
	unsigned char ReservedMaskSize;
	unsigned char ReservedMaskPos;
	unsigned char DirectColorModeInfo;
	unsigned long PhysBasePtr;
	unsigned long OffScreenMemOffset;
	unsigned short OffScreenMemSize;
	unsigned char Reserved[206];
}__attribute__ ((packed)) vbeModeInfo;

bool init_vbe();
void demoVBE(void);
bool showVesaInfo();
void printVbeControllerInfo(vbeControllerInfo * ctrl);
bool isModeInList(unsigned short modeNum, vbeControllerInfo * ctrl);
void printModesWithBBP(unsigned short bbp, vbeControllerInfo * ctrl);

enum VESARet VESAFunc(regs32_t * regs);
enum VESARet getSuperVGAInformation(vbeControllerInfo *ctrl);
enum VESARet getSuperVGAModeInformation(unsigned short modeNum);
enum VESARet setVESAMode(unsigned short modeNum, bool flat);
void apic_irq48_handler(struct regs *r);
bool demoMode(unsigned short modeNum);
#endif
