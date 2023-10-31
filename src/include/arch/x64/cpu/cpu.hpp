/*
   File: include/arch/x64/cpu/cpu.hpp
*/

#pragma once
#include <cstdint.hpp>
#include <dev/cpu.hpp>
#include <arch/x64/interrupts/idt.hpp>

#define MSR_EFER         0xC0000080
#define MSR_STAR         0xC0000081
#define MSR_LSTAR        0xC0000082

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
		usize RAXSave; /* To put the CR3, we can't address GS directly,
				    * so we just use the RAX register and save it here
				    * momentarily
				    */
	}__attribute__((packed));

	struct PerCoreCPUTopology {
		uptr InterruptStack;

		__attribute__((aligned(0x10))) 
		IDTEntry IDT[IDT_MAX_DESCRIPTORS];
		
		__attribute__((aligned(0x10))) 
		IDTR _IDTR;
		
		__attribute__((aligned(0x10))) 
		LocalCPUStruct CPUStruct;
	};

	inline void GetMSR(u32 msr, u32 *lo, u32 *hi) {
		asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
	}
 
	inline void SetMSR(u32 msr, u32 lo, u32 hi) {
		asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
	}

	int BootCPUInit(DEV::CPU::TopologyStructure *core);
	int CurrentCPUInit(DEV::CPU::TopologyStructure *core);

	void CPUInit();
	void UpdateLocalCPUStruct(uptr taskKernelStack, uptr userCR3, uptr kernelCR3);
	const char *GetCPUVendor();
}
