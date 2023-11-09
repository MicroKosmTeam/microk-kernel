/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <cstdint.hpp>
#include <dev/cpu.hpp>
#include <arch/x64/dev/tsc.hpp>
#include <arch/x64/dev/apic.hpp>
#include <arch/x64/dev/ioapic.hpp>
#include <arch/x64/interrupts/idt.hpp>

#define MSR_TSC_VALUE    0x00000010
#define MSR_TSC_ADJUST   0x0000003B
#define MSR_TSC_MPERF    0x000000E7
#define MSR_TSC_APERF    0x000000E8
#define MSR_TSC_AUX      0x00000103
#define MSR_MISC_ENABLE  0x000001A0
#define MSR_TSC_DEADLINE 0x000006E0


#define MSR_EFER         0xC0000080
#define MSR_STAR         0xC0000081
#define MSR_LSTAR        0xC0000082
#define MSR_CSTAR        0xC0000083
#define MSR_FMASK        0xC0000084
#define MSR_FSBASE       0xC0000100
#define MSR_GSBASE       0xC0000101
#define MSR_KERNELGSBASE 0xC0000102

namespace x86_64 {
	struct LocalCPUStruct {
		/* The stack used during a syscall */
		uptr TaskKernelStack;

		/* Where to save the usermode stack during a syscall */
		uptr UserStackPointer;
		uptr UserBaseStackPointer;

		/* For security, the kernel and user have different CR3s,
		 * with the user root page directory having only the bare
		 * kernel essentials mapped in memory.
		 */
		uptr UserCR3;
		uptr KernelCR3;
		usize RAXSave; /* To change the value in CR3
				* we can't address GS directly,
				* so we just use the RAX register and
				* save it here momentarily.
				*/

		uptr TopologyStructure; /* Direct pointer to the current core
					 * topology structure for ease of use.
					 */
	}__attribute__((packed));

	struct IOAPICNode {
		IOAPIC::IOAPIC *IOAPIC;

		IOAPICNode *Next;
		IOAPICNode *Previous;
	};

	struct PerMachineTopology {
		u8 Vendor;

		IOAPICNode *IOAPICList;
	};

	struct PerCoreCPUTopology {
		/* Per-CPU interrupt stack */
		uptr InterruptStack;
		usize InterruptStackSize;

		/* IDT and IDTR, indipendent for each CPU */
		__attribute__((aligned(0x10))) IDTEntry IDT[IDT_MAX_DESCRIPTORS];
		__attribute__((aligned(0x10))) IDTR _IDTR;
	
		/* GDT structures, with a per-CPU TSS */
		__attribute__((aligned(0x10))) GDTPointer GDTPtr;		
		__attribute__((aligned(0x10))) GDT GlobalDescriptorTable;
		__attribute__((aligned(0x10))) TSS TaskStateSegment;

		/* Local CPU structure accessible via GS when in kernel mode */
		LocalCPUStruct CPUStruct;

		/* The LAPIC for the current core */
		APIC::APIC *LocalAPIC;

		/* Time stamp counter for the current processor, if present */
		TSC::TSC *TimeStampCounter;
	};

	inline __attribute__((always_inline))
	void GetMSR(u32 msr, u32 *lo, u32 *hi) {
		asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
	}
 
	inline __attribute__((always_inline))
	void SetMSR(u32 msr, u32 lo, u32 hi) {
		asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
	}

	inline __attribute__((always_inline))
	void GetCoreTopologyStruct(DEV::CPU::TopologyStructure **addr) {
		asm volatile("mov %%gs:48, %0" : "=r"(*addr));
	}

	int BootCPUInit();
	int UpdatePerCPUStack(DEV::CPU::TopologyStructure *core, usize stackSize);
	int CurrentCPUInit(DEV::CPU::TopologyStructure *core);

	void UpdateLocalCPUStruct(LocalCPUStruct *cpuStruct, uptr taskKernelStack, uptr userCR3, uptr kernelCR3);
}
