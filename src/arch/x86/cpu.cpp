#include <cpuid.h>
#include <printk.hpp>
#include <memory.hpp>
#include <kinfo.hpp>
#include <pmm.hpp>
#include <capability.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/idt.hpp>
#include <arch/x86/gdt.hpp>
#include <arch/x86/apic.hpp>

extern "C" void HandleSyscall64();
extern "C" void HandleSyscall32();

namespace x86 {
GDT gdt;
GDTPointer pointer;
TSS tss;
APIC apic;


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
	PRINTK::PrintK(PRINTK_DEBUG "Syscall entries at 0x%x for 64-bit and 0x%x for 32-bit\r\n", &HandleSyscall64, HandleSyscall32);

	u32 msrLo = 0, msrHi = 0;

	/* Enabling syscalls with MSRs, first starting with the SCE enable bit (0) in the EFER MSR */
	GetMSR(MSR_EFER, &msrLo, &msrHi);
	msrLo |= 1;
	SetMSR(MSR_EFER, msrLo, msrHi);

	/* Settign the higher part of the STAR MSR with the GDT offsets (low part is for EIP, unused in 64 bit mode */
	msrLo = (u32)((uptr)&HandleSyscall32 & 0xFFFFFFFF);
	msrHi = (GDT_OFFSET_KERNEL_CODE) | (GDT_OFFSET_USER_CODE << 16);
	SetMSR(MSR_STAR, msrLo, msrHi);

	/* Setting syscall RIP for 64 bit mode in the LSTAR MSR */
	msrLo = (u32)((uptr)&HandleSyscall64 & 0xFFFFFFFF);
	msrHi = (u32)(((uptr)&HandleSyscall64 >> 32) & 0xFFFFFFFF);
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

void LoadEssentialCPUStructures() {
	LoadGDT(&gdt, &pointer);
	TSSInit(&gdt, &tss, (uptr)PMM::RequestPages(8));
	IDTInit();
}

void InitializeCPUFeatures() {
	EnableSyscalls();

	u32 vendor[4];
	Memclr(vendor, sizeof(u32) * 4);
	u32 maxIntelLevel = 0, maxAmdLevel = 0;
	u32 ignored = 0;
	__get_cpuid_count(0, 0, &maxIntelLevel, &vendor[0], &vendor[2], &vendor[1]);
	__get_cpuid_count(0x80000000, 0, &maxAmdLevel, &ignored, &ignored, &ignored);

	PRINTK::PrintK(PRINTK_DEBUG 
			"CPUID max supported Intel level: 0x%x\r\n"
			"CPUID max supported AMD level: 0x%x\r\n"
			"CPUID vendor string: %s\r\n", maxIntelLevel, maxAmdLevel, vendor);

	InitializeAPIC(&apic);
}

void InitializeBootCPU() {
	LoadEssentialCPUStructures();
}

__attribute__((noreturn))
void LoadSchedulerContext(SchedulerContext *context) {
	InterruptStatus *stack = (InterruptStatus*)(context->SP - sizeof(InterruptStatus));

	/* Copy over general registers */
	Memcpy(&stack->Registers, &context->Registers, sizeof(GeneralRegisters));

	/* Set up the interrupt stack frame */
	InterruptStack *intStub = &stack->Base;
	Memclr(intStub, sizeof(InterruptStack));
	intStub->IretRIP = context->IP;
	intStub->IretRSP = context->SP;
	intStub->IretRFLAGS = context->RFLAGS;

	if (!context->InKernel) {
		intStub->IretCS = GDT_OFFSET_USER_CODE;
	} else {
		intStub->IretCS = GDT_OFFSET_KERNEL_CODE;
	}

	asm volatile ("mov %%rsp, %0\n\t"
		      "mov %%rbp, %1\n\t"
		      "pop %%r15\n\t"
		      "pop %%r14\n\t"
		      "pop %%r13\n\t"
		      "pop %%r12\n\t"
		      "pop %%r11\n\t"
		      "pop %%r10\n\t"
		      "pop %%r9\n\t"
		      "pop %%r8\n\t"
		      "pop %%rdx\n\t"
		      "pop %%rcx\n\t"
		      "pop %%rbx\n\t"
		      "pop %%rax\n\t"
		      "pop %%rsi\n\t"
		      "pop %%rdi\n\t"
		      "add %%rsp, 16\n\t"
		      "iretq\n\t" : : "r"(stack), "r"(context->BP));

	__builtin_unreachable();
}
}
