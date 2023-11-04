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
#include <mm/bootmem.hpp>

extern "C" void SyscallEntry();

extern "C" void EnableSSE();
extern "C" void EnableAVX();
extern "C" void EnableSCE(void *compatSyscallEntry, void *syscallEntry); 

extern "C" void SetIOPL();

namespace x86_64 {
inline static u32 GetFamily(u32 sig) {
	u32 x86;

	x86 = (sig >> 8) & 0xf;

	if (x86 == 0xf)
		x86 += (sig >> 20) & 0xff;

	return x86;
}

inline static u32 GetModel(u32 sig) {
	u32 fam, model;

	fam = GetFamily(sig);

	model = (sig >> 4) & 0xf;

	if (fam >= 0x6)
		model += ((sig >> 16) & 0xf) << 4;

	return model;
}

inline static u32 GetStepping(u32 sig) {
	return sig & 0xf;
}

inline static int EnableSyscalls() {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Syscall entries at 0x%x\r\n", &SyscallEntry);

	u32 msrLo = 0, msrHi = 0;

	/* Enabling syscalls with MSRs, first starting with the SCE enable bit (0) in the EFER MSR */
	GetMSR(MSR_EFER, &msrLo, &msrHi);
	msrLo |= 1;
	SetMSR(MSR_EFER, msrLo, msrHi);

	/* Settign the higher part of the STAR MSR with the GDT offsets (low part is for EIP, unused in 64 bit mode */
	msrLo = 0;
	msrHi = (GDT_OFFSET_KERNEL_CODE) | (GDT_OFFSET_USER_CODE << 16);
	SetMSR(MSR_STAR, msrLo, msrHi);

	/* Setting syscall RIP for 64 bit mode in the LSTAR MSR */
	msrLo = (uptr)&SyscallEntry;
	msrHi = (uptr)&SyscallEntry >> 32;
	SetMSR(MSR_LSTAR, msrLo, msrHi);

	/* Setting syscall RIP for compatibility mode in the CSTAR MSR (for now it's 0, not supported */
	msrLo = 0xDEADC0DE;
	msrHi = 0x0;
	SetMSR(MSR_CSTAR, msrLo, msrHi);

	/* Setting RFLAGS mask to 0 in the FMASK MSR, no masking necessary */
	msrLo = 0;
	msrHi = 0;
	SetMSR(MSR_FMASK, msrLo, msrHi);

	return 0;
}


int BootCPUInit() {
	KInfo *info = GetInfo();
	DEV::CPU::TopologyStructure *machine = info->DefaultMachine;
	DEV::CPU::TopologyStructure *core = info->BootCore;

	PerMachineTopology *machineInfo = (PerMachineTopology*)BOOTMEM::Malloc(sizeof(PerMachineTopology));
	PerCoreCPUTopology *coreInfo = (PerCoreCPUTopology*)BOOTMEM::Malloc(sizeof(PerCoreCPUTopology));
	machine->ArchitectureSpecificInformation = (void*)machineInfo;
	core->ArchitectureSpecificInformation = (void*)coreInfo;

	/* We first of all get the position of the kernel interrupt stack and save it
	 * as we will use it to initialize the TSS. We start from 640K in x86_64 until
	 * we allocate a proper location, as we know it will always be free for use.
	 */
	coreInfo->InterruptStack = VMM::PhysicalToVirtual(640 * 1024); 
	coreInfo->InterruptStackSize = 640 * 1024 - PAGE_SIZE;
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Temporary kernel stack: 0x%x\r\n", coreInfo->InterruptStack);

	/* Initialize the GDT */
	LoadGDT(&coreInfo->GlobalDescriptorTable, &coreInfo->GDTPtr);

	/* Initialization of the TSS */
	TSSInit(&coreInfo->GlobalDescriptorTable, &coreInfo->TaskStateSegment, coreInfo->InterruptStack);

	/* Jumpstart interrupts */
	IDTInit((IDTEntry*)coreInfo->IDT, &coreInfo->_IDTR);
	
	/* Initializing the user GSBASE MSR to 0 */
	SetMSR(MSR_GSBASE, 0, 0);

	SetMSR(MSR_KERNELGSBASE, (uptr)&coreInfo->CPUStruct, (uptr)&coreInfo->CPUStruct >> 32);
	UpdateLocalCPUStruct(&coreInfo->CPUStruct, 0, VMM::VirtualToPhysical(info->KernelVirtualSpace), VMM::VirtualToPhysical(info->KernelVirtualSpace));

	coreInfo->CPUStruct.TopologyStructure = (uptr)core;

	return 0;
}
	
int UpdatePerCPUStack(DEV::CPU::TopologyStructure *core, usize stackSize) {
	PerCoreCPUTopology *coreInfo = (PerCoreCPUTopology*)core->ArchitectureSpecificInformation;

	PMM::FreePages((void*)(coreInfo->InterruptStack - coreInfo->InterruptStackSize),
			coreInfo->InterruptStackSize / PAGE_SIZE);

	coreInfo->InterruptStackSize = stackSize;
	coreInfo->InterruptStack = VMM::PhysicalToVirtual((uptr)PMM::RequestPages(stackSize / PAGE_SIZE) + stackSize);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPU interrupt stack: 0x%x\r\n", coreInfo->InterruptStack);

	/* Initialization of the TSS */
	LoadNewStackInTSS(&coreInfo->TaskStateSegment, coreInfo->InterruptStack);

	return 0;
}
	
int CurrentCPUInit(DEV::CPU::TopologyStructure *core) {
	KInfo *info = GetInfo();
	PerCoreCPUTopology *coreInfo = (PerCoreCPUTopology*)core->ArchitectureSpecificInformation; 

	/* Initialize the GDT */
	LoadGDT(&coreInfo->GlobalDescriptorTable, &coreInfo->GDTPtr);

	/* Initialization of the TSS */
	TSSInit(&coreInfo->GlobalDescriptorTable, &coreInfo->TaskStateSegment, coreInfo->InterruptStack);

	/* Jumpstart interrupts */
	IDTInit((IDTEntry*)coreInfo->IDT, &coreInfo->_IDTR);
	
	u32 vendor[4];
	Memset(vendor, 0, sizeof(u32) * 4);
	u32 maxIntelLevel = 0, maxAmdLevel = 0;
	u32 ignored = 0;
	u32 feature;
	__get_cpuid(0, &maxIntelLevel, &vendor[0], &vendor[2], &vendor[1]);
	__get_cpuid(0x80000000, &maxAmdLevel, &ignored, &ignored, &ignored);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME,
			"CPUID max supported Intel level: 0x%x\r\n"
			"CPUID max supported AMD level: 0x%x\r\n"
			"CPUID vendor string: %s\r\n", maxIntelLevel, maxAmdLevel, vendor);

	if (maxIntelLevel >= 0x00000001 &&
	    maxIntelLevel <= 0x0000ffff) {
		u32 vendorInfo = 0;
		__get_cpuid(1, &vendorInfo, &ignored, &ignored, &ignored);
		u32 family, model, stepping;
		family = GetFamily(vendorInfo);
		model = GetModel(vendorInfo);
		stepping = GetStepping(vendorInfo);

		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPUID processor info:\r\n"
			"Family:   0x%x\r\n"
			"Model:    0x%x\r\n"
			"Stepping: 0x%x\r\n"
			, family, model, stepping);

	}

	if (maxIntelLevel >= 0x00000007) {
		__get_cpuid(0x00000007,  &ignored, &ignored, &feature, &ignored);
	}

	if (maxAmdLevel >= 0x80000001 &&
	    maxAmdLevel <= 0x8000ffff) {
		__get_cpuid(0x80000001, &ignored, &ignored, &feature, &feature);
	}

	SetIOPL();

	EnableSyscalls();

	/* Initializing the user GSBASE MSR to 0 */
	SetMSR(MSR_GSBASE, 0, 0);

	SetMSR(MSR_KERNELGSBASE, (uptr)&coreInfo->CPUStruct, (uptr)&coreInfo->CPUStruct >> 32);
	UpdateLocalCPUStruct(&coreInfo->CPUStruct, 0, VMM::VirtualToPhysical(info->KernelVirtualSpace), VMM::VirtualToPhysical(info->KernelVirtualSpace));

	coreInfo->CPUStruct.TopologyStructure = (uptr)core;


	return 0;
}
/*
   Function that initializes the x86CPU class
*/

void UpdateLocalCPUStruct(LocalCPUStruct *cpuStruct, uptr taskKernelStack, uptr userCR3, uptr kernelCR3) {
	cpuStruct->TaskKernelStack = taskKernelStack;
	cpuStruct->UserCR3 = userCR3;
	cpuStruct->KernelCR3 = kernelCR3;
}

}
