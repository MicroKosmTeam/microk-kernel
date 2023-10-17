/*
   File: arch/x64/cpu/cpu.cpp
*/

#include <cpuid.h>
#include <sys/printk.hpp>
#include <arch/x64/cpu/cpu.hpp>
#include <sys/panic.hpp>
#include <cstdint.hpp>
#include <mm/pmm.hpp>

#include <mm/string.hpp>
#include <init/kinfo.hpp>

extern "C" void SyscallEntry();

extern "C" void EnableSSE();
extern "C" void EnableAVX();
extern "C" void EnableSCE(void *compatSyscallEntry, void *syscallEntry); 

extern "C" void SetIOPL();

#define CPU_VENDOR_LENGTH 13 /* 12 + NULL */

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



static u16 EnableSMID() {
	unsigned int eax, ebx, ecx, edx;
	__cpuid(1, eax, ebx, ecx, edx);

	u16 result = 0;

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

uptr localCPUStruct = 0;
void CPUInit() {
	KInfo *info = GetInfo();

	SetIOPL();

	u16 sseFeatures = EnableSMID();
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "SSE features: %x\r\n", sseFeatures);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Syscall entries at 00x%x\r\n", &SyscallEntry);
	EnableSCE(NULL, (void*)&SyscallEntry);

	SetMSR(MSR_GSBASE, 0xDEADDEAD, 0xDEADDEAD);

	localCPUStruct = (uptr)PMM::RequestPage() + info->HigherHalfMapping;
	SetMSR(MSR_KERNELGSBASE, localCPUStruct, localCPUStruct >> 32);

	UpdateLocalCPUStruct(0xDEADC0DECAFEBABE,
			     (uptr)info->KernelVirtualSpace->GetTopAddress() - info->HigherHalfMapping,
			     (uptr)info->KernelVirtualSpace->GetTopAddress() - info->HigherHalfMapping);
}

void UpdateLocalCPUStruct(uptr taskKernelStack, uptr userCR3, uptr kernelCR3) {
	LocalCPUStruct *cpuStruct = (LocalCPUStruct*)localCPUStruct;

	cpuStruct->TaskKernelStack = taskKernelStack;
	cpuStruct->UserCR3 = userCR3;
	cpuStruct->KernelCR3 = kernelCR3;
}

/*
   Function that gets the CPU vendor string from CPUID
*/
const char *GetCPUVendor() {
	u32 ebx, edx, ecx, unused;
	__cpuid(0, unused, ebx, ecx, edx);

	char string[CPU_VENDOR_LENGTH] = { 0 };

	/* This bitshift logic is used to get the correct 8-bit characters
	 * from the 32 bit registers. The vendor name is 12 characters + NULL
	 */
	string[0] = (u8)(ebx & 0xFF);
	string[1] = (u8)((ebx >> 8) & 0xFF);
	string[2] = (u8)((ebx >> 16) & 0xFF);
	string[3] = (u8)((ebx >> 24) & 0xFF);
	string[4] = (u8)(edx & 0xFF);
	string[5] = (u8)((edx >> 8) & 0xFF);
	string[6] = (u8)((edx >> 16) & 0xFF);
	string[7] = (u8)((edx >> 24) & 0xFF);
	string[8] = (u8)(ecx & 0xFF);
	string[9] = (u8)((ecx >> 8) & 0xFF);
	string[10] = (u8)((ecx >> 16) & 0xFF);
	string[11] = (u8)((ecx >> 24) & 0xFF);
	string[12] = '\0';

	usize index = 0;
	while(CPUVendorStrings[index] != NULL) {
		if(Strncmp(string, CPUVendorStrings[index], CPU_VENDOR_LENGTH) == 0) return CPUVendorStrings[index];

		++index;
	}

	return CPUVendorStrings[index + 1];
}
}
