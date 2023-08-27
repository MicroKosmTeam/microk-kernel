/*
   File: arch/x64/interrupts/idt.cpp
*/

#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>
#include <sys/user.hpp>
#include <sys/syscall.hpp>
#include <arch/x64/interrupts/idt.hpp>

/* Setting the kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

/* Max number of interrupts in x86_64 is 256 */
#define IDT_MAX_DESCRIPTORS 256

/* Create the IDT, aligned for maximum performance */
volatile __attribute__((aligned(0x10))) IDTEntry idt[IDT_MAX_DESCRIPTORS];
/* Create the IDTR */
volatile __attribute__((aligned(0x10))) IDTR idtr;

/* Function to set a descriptor in the GDT */
static void IDTSetDescriptor(uint8_t vector, void* isr, uint8_t flags) {
	/* Create new descriptor */
	IDTEntry* descriptor = &idt[vector];

	/* Setting parameters */
	descriptor->ISRLow = (uint64_t)isr & 0xFFFF;
	descriptor->KernelCs = GDT_OFFSET_KERNEL_CODE;
	descriptor->IST = 0;
	descriptor->Attributes = flags;
	descriptor->ISRMid = ((uint64_t)isr >> 16) & 0xFFFF;
	descriptor->ISRHigh = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->Reserved0 = 0;
}

/* ISR stub table, declared in assembly code */
extern void *isrStubTable[];

namespace x86_64 {
/* Function to initialize the IDT */
void IDTInit() {
	/* Set base and size in the pointer */
	idtr.Base = (uintptr_t)&idt[0];
	idtr.Limit = (uint16_t)sizeof(IDTEntry) * IDT_MAX_DESCRIPTORS - 1;

	/* Fill in the 32 exception handlers */
	for (uint8_t vector = 0; vector < 32; vector++) {
		IDTSetDescriptor(vector, isrStubTable[vector],  0x8F);
	}
		
	IDTSetDescriptor(32, isrStubTable[32], 0x8E);
	IDTSetDescriptor(254, isrStubTable[254], 0xEE);

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

#include <arch/x64/dev/apic.hpp>
/* Stub exception handler */
extern "C" CPUStatus *InterruptHandler(CPUStatus *context) {
	KInfo *info = GetInfo();
	bool isFatal = false;

	uintptr_t cr3;
	asm volatile("mov %%cr3, %0" : "=r"(cr3) :: "memory");
	bool switchAddressSpace = (cr3 != info->kernelVirtualSpace->GetTopAddress()) ? true : false;

	PROC::Process *proc;
	VMM::VirtualSpace *procSpace;

	if(switchAddressSpace) {
		VMM::LoadVirtualSpace(info->kernelVirtualSpace);

		proc = info->kernelScheduler->GetRunningProcess();
		if(proc != NULL) procSpace = proc->GetVirtualMemorySpace();
		else PANIC("Null proc");
	}

	switch(context->VectorNumber) {
		case 0:
			PRINTK::PrintK("Division by zero.\r\n");
			break;
		case 6:
			isFatal = true;
			PRINTK::PrintK("Invalid opcode.\r\n");
			break;
		case 8:
			PANIC("Double fault");
			break;
		case 13: {
			isFatal = true;
			PRINTK::PrintK("General protection fault.\r\n");
			break;
			}
		case 14: {
			isFatal = true;
			uintptr_t page;
			bool protectionViolation = context->ErrorCode & 0b1;
			bool writeAccess = (context->ErrorCode & 0b10) >> 1;
			bool byUser = (context->ErrorCode & 0b100) >> 2;
			bool wasInstructionFetch = (context->ErrorCode & 0b1000) >> 4;
			asm("mov %%cr2,%0" : "=r"(page));
			PRINTK::PrintK("Page fault in page 0x%x because of a %s.\r\nIt was caused by a %s from %s.\r\nIt %s because of an instruction fetch.\r\n",
					page,
					protectionViolation ? "page protection violation" : "non-present page",
					writeAccess ? "write" : "read",
					byUser ? "userspace" : "kernelspace",
					wasInstructionFetch ? "was" : "wasn't"
					);

			}
			break;
		case 32:
			x86_64::SendAPICEOI();
			x86_64::WaitAPIC();
			/* TODO:
			if(info->kernelScheduler != NULL)
				info->kernelScheduler->RecalculateScheduler();
			*/
			break;
		case 254:
			HandleSyscall(context->RAX, context->RDI, context->RSI, context->RDX, context->RCX, context->R8, context->R9);
			break;
		default:
			isFatal = true;
			PRINTK::PrintK("Unhandled exception: %d\r\n", context->VectorNumber);
			break;
	}

	if(context->IretCS == GDT_OFFSET_KERNEL_CODE) {
		if (isFatal) {
			PRINTK::PrintK("\r\n\r\n--[cut here]--\r\n"
					"Exception in CPU.\r\n");

			PrintRegs(context);
			PANIC("Kernel mode exception");
		} else {
			/* Do nothing */
		}
	} else {
		if (isFatal) {
			PRINTK::PrintK("\r\n\r\n--[cut here]--\r\n"
					"Exception in CPU.\r\n");

			PrintRegs(context);

			info->kernelScheduler->SetProcessState(proc->GetPID(), PROC::P_WAITING);

			while(true) {
				info->kernelScheduler->RecalculateScheduler();
			}
		} else {
			context->IretCS = GDT_OFFSET_USER_CODE;
			context->IretSS = GDT_OFFSET_USER_CODE + 0x08;
		}
	}

	if(switchAddressSpace) {
		VMM::LoadVirtualSpace(procSpace);
	}
	
	return context;
}
