/*
   File: arch/x64/cpu/cpu.cpp
*/

#include <cpuid.h>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <sys/panic.hpp>
#include <stdint.h>
#include <mm/pmm.hpp>
#include <stddef.h>
#include <mm/string.hpp>
#include <init/kinfo.hpp>

extern "C" void SyscallEntry();

extern "C" void EnableSCE(void *syscallEntrypoint);
extern "C" void EnableSSE();
extern "C" void EnableAVX();

extern "C" void SetIOPL();

static const char *CPUVendorStrings[] {
	"AuthenticAMD",
	"AMDisbetter!",
	"GenuineIntel",
	"VIA VIA VIA ",
	"GenuineTMx86",
	"TransmetaCPU",
	"CyrixInstead",
	"CentaurHauls",
	"NexGenDriven",
	"UMC UMC UMC ",
	"SiS SiS SiS ",
	"Geode by NSC",
	"RiseRiseRise",
	"Vortex86 SoC",
	"MiSTer AO486",
	"GenuineAO486",
	"  Shanghai  ",
	"HygonGenuine",
	"E2K MACHINE ",
	"TCGTCGTCGTCG",
	" KVMKVMKVM  ",
	"VMwareVMware",
	"VBoxVBoxVBox",
	"XenVMMXenVMM",
	"Microsoft Hv",
	" prl hyperv ",
	" lrpepyh vr ",
	"bhyve bhyve ",
	" QNXQVMBSQG ",
	NULL,
	"Unknown"
};



static uint16_t EnableSMID() {
	unsigned int eax, ebx, ecx, edx;
	__cpuid(1, eax, ebx, ecx, edx);

	uint16_t result = 0;

	/* SSE */
	if(edx & (1 << 25)) {
		/* Enable SSE */
		EnableSSE();

		result |= (1 << 0);

		/* SSE2 */
		if(edx & (1 << 26)) {
			result |= (1 << 1);
		}

		/* SSE3 */
		if(ecx & (1 << 0)) {
			result |= (1 << 2);

			/* SSSE3 */
			if(ecx & (1 << 9)) {
				result |= (1 << 3);
			}
		}

		/* SSE4.1 */
		if(ecx & (1 << 19)) {
			result |= (1 << 4);

			/* SSE4.2 */
			if(ecx & (1 << 20)) {
				result |= (1 << 5);
			}

			/* SSE4A */
			if(ecx & (1 << 6)) {
				result |= (1 << 6);
			}
		}
	

		/* XSAVE */
		if(ecx & (1 << 26)) {
//			Not yet decided
			result |= (1 << 7);

	
		/* AVX */
		if(ecx & (1 << 28)) {
//			Not yet decided
			result |= (1 << 8);

			EnableAVX();
		}

		}
	}

	return result;
}

namespace x86_64 {
/*
   Function that initializes the x86CPU class
*/

uintptr_t localCPUStruct = 0;
void CPUInit() {
	KInfo *info = GetInfo();

	SetIOPL();

	uint16_t sseFeatures = EnableSMID();
	PRINTK::PrintK("SSE features: %x\r\n", sseFeatures);

	PRINTK::PrintK("Syscall entry at 0x%x\r\n", &SyscallEntry);
	EnableSCE((void*)&SyscallEntry);

	SetMSR(MSR_GSBASE, 0xDEADDEAD, 0xDEADDEAD);

	localCPUStruct = (uintptr_t)PMM::RequestPage() + info->HigherHalfMapping;
	SetMSR(MSR_KERNELGSBASE, localCPUStruct, localCPUStruct >> 32);

	UpdateLocalCPUStruct(0xDEADC0DECAFEBABE);
}

void UpdateLocalCPUStruct(uintptr_t taskKernelStack) {
	LocalCPUStruct *cpuStruct = (LocalCPUStruct*)localCPUStruct;
	Memset(cpuStruct, 0, sizeof(LocalCPUStruct));

	PRINTK::PrintK("New kernel stack: 0x%x\r\n", taskKernelStack);
	cpuStruct->TaskKernelStack = taskKernelStack;
}

/*
   Function that gets the CPU vendor string from CPUID
*/
const char *GetCPUVendor() {
	uint32_t ebx, edx, ecx, unused;
	__cpuid(0, unused, ebx, ecx, edx);

	char string[13];

	/* This bitshift logic is used to get the correct 8-bit characters
	   from the 32 bit registers. The vendor name is 12 characters + \0 */
	string[0] = (uint8_t)(ebx & 0xFF);
	string[1] = (uint8_t)((ebx >> 8) & 0xFF);
	string[2] = (uint8_t)((ebx >> 16) & 0xFF);
	string[3] = (uint8_t)((ebx >> 24) & 0xFF);
	string[4] = (uint8_t)(edx & 0xFF);
	string[5] = (uint8_t)((edx >> 8) & 0xFF);
	string[6] = (uint8_t)((edx >> 16) & 0xFF);
	string[7] = (uint8_t)((edx >> 24) & 0xFF);
	string[8] = (uint8_t)(ecx & 0xFF);
	string[9] = (uint8_t)((ecx >> 8) & 0xFF);
	string[10] = (uint8_t)((ecx >> 16) & 0xFF);
	string[11] = (uint8_t)((ecx >> 24) & 0xFF);
	string[12] = '\0';

	size_t index = 0;
	while(CPUVendorStrings[index] != NULL) {
		if(strcmp(string, CPUVendorStrings[index]) == 0) return CPUVendorStrings[index];

		++index;
	}

	return CPUVendorStrings[index + 1];
}
}
