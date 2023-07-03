/*
   File: arch/x64/interrupts/idt.cpp
*/

#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/interrupts/idt.hpp>

/* Setting the kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
/* Max number of interrupts in x86_64 is 256 */
#define IDT_MAX_DESCRIPTORS 256

/* Create the IDT, aligned for maximum performance */
__attribute__((aligned(0x10))) IDTEntry idt[IDT_MAX_DESCRIPTORS];
/* Create the IDTR */
IDTR idtr;

/* Function to set a descriptor in the GDT */
static void IDTSetDescriptor(uint8_t vector, void* isr, uint8_t flags) {
	/* Create new descriptor */
	IDTEntry* descriptor = &idt[vector];

	/* Setting parameters */
	descriptor->isrLow = (uint64_t)isr & 0xFFFF;
	descriptor->kernelCs = GDT_OFFSET_KERNEL_CODE;
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isrMid = ((uint64_t)isr >> 16) & 0xFFFF;
	descriptor->isrHigh = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

/* ISR stub table, declared in assembly code */
extern void *isrStubTable[];

namespace x86_64 {
/* Function to initialize the IDT */
void IDTInit() {
	/* Set base and size in the pointer */
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(IDTEntry) * IDT_MAX_DESCRIPTORS - 1;

	/* Fill in the 32 exception handlers */
	for (uint8_t vector = 0; vector < 32; vector++) {
		IDTSetDescriptor(vector, isrStubTable[vector],  0x8F);
	}
		
	IDTSetDescriptor(32, isrStubTable[32],   0b11101110);
	IDTSetDescriptor(39, isrStubTable[39],   0b11101110);
	IDTSetDescriptor(254, isrStubTable[254], 0b11101110);

	/* Load the new IDT */
	asm volatile ("lidt %0" : : "m"(idtr));
	/* Set the interrupt flag */
	asm volatile ("sti");
}
}

static inline void PrintRegs(CPUStatus *context) {
	PRINTK::PrintK(" -> RAX: 0x%x\r\n"
			" -> RBX: 0x%x\r\n"
			" -> RCX: 0x%x\r\n"
			" -> RDX: 0x%x\r\n"
			" -> R08: 0x%x\r\n"
			" -> R09: 0x%x\r\n"
			" -> R10: 0x%x\r\n"
			" -> R11: 0x%x\r\n"
			" -> R12: 0x%x\r\n"
			" -> R13: 0x%x\r\n"
			" -> R14: 0x%x\r\n"
			" -> R15: 0x%x\r\n"
			" -> RDI: 0x%x\r\n"
			" -> RSI: 0x%x\r\n\r\n"
			" Vector Number: 0x%x\r\n"
			" Error Code: 0x%x\r\n"
			" Iret RIP: 0x%x\r\n"
			" Iret CS: 0x%x\r\n"
			" Iret RFLAGS: 0x%x\r\n"
			" Iret RSP: 0x%x\r\n"
			" Iret SS: 0x%x\r\n",
			context->RAX,
			context->RBX,
			context->RCX,
			context->RDX,
			context->R8,
			context->R9,
			context->R10,
			context->R11,
			context->R12,
			context->R13,
			context->R14,
			context->R15,
			context->RDI,
			context->RSI,
			context->VectorNumber,
			context->ErrorCode,
			context->IretRIP,
			context->IretCS,
			context->IretRFLAGS,
			context->IretRSP,
			context->IretSS);
}

/* Stub exception handler */
extern "C" CPUStatus *exceptionHandler(CPUStatus *context) {
	PRINTK::PrintK("\r\n\r\n\r\n EXCEPTION!!\r\n");

	PrintRegs(context);

	if (context->IretCS == GDT_OFFSET_KERNEL_CODE) PANIC("Exception");
	else OOPS("User exception");

	switch(context->VectorNumber) {
		case 13:
			PRINTK::PrintK("General protection fault\r\n");
			break;
		case 14: {
			uintptr_t page;
			asm("mov %%cr2,%0" : "=r"(page));
			PRINTK::PrintK("Page fault in page 0x%x\r\n", page);
			}
			break;
		default:
			PRINTK::PrintK("Unhandled exception\r\n");
			break;
	}
	
	return context;
}

#include <arch/x64/dev/apic.hpp>
#include <mm/vmm.hpp>
extern "C" CPUStatus *timerHandler(CPUStatus *context) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);	
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	x86_64::WaitAPIC(0x10000);
	x86_64::SendAPICEOI();

	VMM::LoadVirtualSpace(procSpace);
	
	return context;
}

extern "C" CPUStatus *spuriousHandler(CPUStatus *context) {
	PRINTK::PrintK("Spurious.\r\n");

	return context;
}
#include <sys/user.hpp>
#include <sys/syscall.hpp>
extern "C" CPUStatus *syscallHandler(CPUStatus *context) {
	KInfo *info = GetInfo();

	HandleSyscall(context->RAX, context->RDI, context->RSI, context->RDX, context->RCX, context->R8, context->R9);

	/*
	context->IretRIP = HandleSyscall;
	context->IretCS = GDT_OFFSET_KERNEL_CODE;
	context->IretRSP = &testStack[8191];
	context->IretSS = GDT_OFFSET_KERNEL_CODE + 0x8;*/

	return context;
}
