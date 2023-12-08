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
	struct InterruptContext {
		u64 VectorNumber;
		u64 ErrorCode;

		u64 IP;
		u64 CS;
		u64 RFLAGS;
		u64 SP;
		u64 SS;
	} __attribute__((packed));

	struct CommonRegisterContext {
		u64 R15;
		u64 R14;
		u64 R13;
		u64 R12;
		u64 R11;
		u64 R10;
		u64 R9;
		u64 R8;

		u64 RDX;
		u64 RCX;
		u64 RBX;
		u64 RAX;

		u64 RSI;
		u64 RDI;
	} __attribute__((packed));

	struct SIMDContext {
		u64 ZMM31[8];
		u64 ZMM30[8];
		u64 ZMM29[8];
		u64 ZMM28[8];
		u64 ZMM27[8];
		u64 ZMM26[8];
		u64 ZMM25[8];
		u64 ZMM24[8];
		u64 ZMM23[8];
		u64 ZMM22[8];
		u64 ZMM21[8];
		u64 ZMM20[8];
		u64 ZMM19[8];
		u64 ZMM18[8];
		u64 ZMM17[8];
		u64 ZMM16[8];
		u64 ZMM15[8];
		u64 ZMM14[8];
		u64 ZMM13[8];
		u64 ZMM12[8];
		u64 ZMM11[8];
		u64 ZMM10[8];
		u64 ZMM9[8];
		u64 ZMM8[8];
		u64 ZMM7[8];
		u64 ZMM6[8];
		u64 ZMM5[8];
		u64 ZMM4[8];
		u64 ZMM3[8];
		u64 ZMM2[8];
		u64 ZMM1[8];
		u64 ZMM0[8];
	} __attribute__((packed));

	struct BasicCPUContext {
		SIMDContext SIMDRegisters;
		CommonRegisterContext CommonRegisters;
		InterruptContext InterruptStub;
	} __attribute__((packed));

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

		uptr ACPI_TableMADT;
		uptr ACPI_TableHPET;
		uptr ACPI_TableSRAT;

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

	inline __attribute__((always_inline))
	usize GetContextSize() {
		return sizeof(BasicCPUContext);
	}
	
	int BootCPUInit();
	int UpdatePerCPUStack(DEV::CPU::TopologyStructure *core, usize stackSize);
	int CurrentCPUInit(DEV::CPU::TopologyStructure *core);

	void UpdateLocalCPUStruct(LocalCPUStruct *cpuStruct, uptr taskKernelStack, uptr userCR3, uptr kernelCR3);
}
