/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <cstdint.hpp>
#include <dev/cpu.hpp>
#include <arch/x64/cpu/msr.hpp>
#include <arch/x64/dev/tsc.hpp>
#include <arch/x64/dev/apic.hpp>
#include <arch/x64/cpu/flags.hpp>
#include <arch/x64/dev/ioapic.hpp>
#include <arch/x64/interrupts/idt.hpp>

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


		u32 CPUIDFlags[3];
	};

	inline __attribute__((always_inline))
	void GetCoreTopologyStruct(DEV::CPU::TopologyStructure **addr) {
		asm volatile("mov %%gs:48, %0" : "=r"(*addr));
	}

	inline __attribute__((always_inline))
	uptr GetCurrentStackPointer() {
		uptr stackPtr;
		asm volatile ("mov %%rsp, %0" : "=r"(stackPtr));
		return stackPtr;
	}

	inline __attribute__((always_inline))
	uptr GetCurrentStackBase() {
		uptr stackPtr;
		asm volatile ("mov %%rbp, %0" : "=r"(stackPtr));
		return stackPtr;
	}


	int BootCPUInit();
	int UpdatePerCPUStack(DEV::CPU::TopologyStructure *core, usize stackSize);
	int CurrentCPUInit(DEV::CPU::TopologyStructure *core);

	void UpdateLocalCPUStruct(LocalCPUStruct *cpuStruct, uptr taskKernelStack, uptr userCR3, uptr kernelCR3);
}
