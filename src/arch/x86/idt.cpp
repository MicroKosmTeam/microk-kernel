#include <sched.hpp>
#include <panic.hpp>
#include <printk.hpp>
#include <kinfo.hpp>
#include <vmm.hpp>
#include <pmm.hpp>
#include <arch/x86/idt.hpp>

/* ISR stub table, declared in assembly code */
extern "C" void *ISRStubTable[];
extern "C" x86::InterruptStatus *InterruptHandler(x86::InterruptStatus *context);

namespace x86 {
__attribute__((section(".interrupt"), aligned(0x10))) IDTEntry IDT[IDT_MAX_DESCRIPTORS];
__attribute__((section(".interrupt"), aligned(0x10))) IDTR _IDTR;

static inline void PrintRegs(InterruptStatus *context) {
	PRINTK::PrintK(PRINTK_DEBUG " -> RAX: 0x%x\r\n"
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
			context->Registers.RAX,
			context->Registers.RBX,
			context->Registers.RCX,
			context->Registers.RDX,
			context->Registers.R8,
			context->Registers.R9,
			context->Registers.R10,
			context->Registers.R11,
			context->Registers.R12,
			context->Registers.R13,
			context->Registers.R14,
			context->Registers.R15,
			context->Registers.RDI,
			context->Registers.RSI,
			context->Base.VectorNumber,
			context->Base.ErrorCode,
			context->Base.IretRIP,
			context->Base.IretCS,
			context->Base.IretRFLAGS,
			context->Base.IretRSP,
			context->Base.IretSS);
}

/* Function to set a descriptor in the IDT */
void IDTSetDescriptor(u8 vector, void *isr, u8 ist, u8 flags) {
	/* Create new descriptor */
	volatile IDTEntry *descriptor = &IDT[vector];

	/* Setting parameters */
	descriptor->ISRLow = (u64)isr & 0xFFFF;
	descriptor->KernelCs = GDT_OFFSET_KERNEL_CODE;
	descriptor->IST = ist;
	descriptor->Attributes = flags;
	descriptor->ISRMid = ((u64)isr >> 16) & 0xFFFF;
	descriptor->ISRHigh = ((u64)isr >> 32) & 0xFFFFFFFF;
	descriptor->Reserved0 = 0;
}

/* Function to initialize the IDT */
void IDTInit() {
	/* Set base and size in the pointer */
	_IDTR.Base = (uptr)&IDT[0];
	_IDTR.Limit = (u16)sizeof(IDTEntry) * IDT_MAX_DESCRIPTORS - 1;

	/* Fill in the 32 exception handlers */
	for (u8 vector = 0; vector < 32; vector++) {
		IDTSetDescriptor(vector, ISRStubTable[vector], 0, 0x8E);
	}
	
	/* Load the new IDT */
	asm volatile ("lidt %0" : : "m"(_IDTR));

	/* Set the interrupt flag */
	asm volatile ("sti");
}


extern "C" InterruptStatus *InterruptHandler(InterruptStatus *context) {
	KInfo *info = GetInfo();

	switch(context->Base.VectorNumber) {
		case 0:
			PRINTK::PrintK(PRINTK_DEBUG "Division by zero.\r\n");
			break;
		case 1:
			PRINTK::PrintK(PRINTK_DEBUG "Trap\r\n");
			PrintRegs(context);
			break;
		case 6:
			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG "Invalid opcode: 0x%x.\r\n", *(u8*)context->Base.IretRIP);
			break;
		case 8:
			PrintRegs(context);
			PANIC("Double fault");
			break;
		case 13: {
			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG "General protection fault.\r\n");
			PANIC("General protection fault");
			break;
			}
		case 14: {
			Scheduler *scheduler = info->BootDomain->DomainScheduler;
			ThreadControlBlock *activeThread = scheduler->Running;

			uptr page = 0;
			bool protectionViolation = context->Base.ErrorCode & 0b1;
			bool writeAccess = (context->Base.ErrorCode & 0b10) >> 1;
			bool byUser = (context->Base.ErrorCode & 0b100) >> 2;
			bool wasInstructionFetch = (context->Base.ErrorCode & 0b1000) >> 4;
			asm volatile("mov %%cr2, %0" : "=r"(page));

			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG
				       "Address space: 0x%x\r\n"
				       "Page fault in page 0x%x because of a %s.\r\n"
				       "It was caused by a %s from %s.\r\n"
				       "It %s because of an instruction fetch.\r\n",
				       activeThread->MemorySpace, page,
				       protectionViolation ? "page protection violation" : "non-present page",
				       writeAccess ? "write" : "read",
				       byUser ? "userspace" : "Kernelspace",
				       wasInstructionFetch ? "was" : "wasn't");


			if (byUser) {
				volatile u64 *pte = x86_64::FindMappedPTE(activeThread->MemorySpace, page, false);
				if (*pte & VMM_FLAGS_USER) {
					if (writeAccess) {
						if (*pte & VMM_FLAGS_WRITE) {
							if (*pte & VMM_FLAGS_READ) {
								PRINTK::PrintK(PRINTK_DEBUG "Write -> RW\r\n");
							} else {
								PRINTK::PrintK(PRINTK_DEBUG "Write -> Write\r\n");
							}
						} else if (*pte & VMM_FLAGS_READ) {
							PRINTK::PrintK(PRINTK_DEBUG "Write -> Read\r\n");
						} else {
							PRINTK::PrintK(PRINTK_DEBUG "Write -> Nil\r\n");
						}
					} else {
						if (*pte & VMM_FLAGS_WRITE) {
							if (*pte & VMM_FLAGS_READ) {
								PRINTK::PrintK(PRINTK_DEBUG "Read -> RW\r\n");
							} else {
								PRINTK::PrintK(PRINTK_DEBUG "Read -> Write\r\n");
							}
						} else if (*pte & VMM_FLAGS_READ) {
							PRINTK::PrintK(PRINTK_DEBUG "Read -> Read\r\n");
						} else {
							PRINTK::PrintK(PRINTK_DEBUG "Read -> Nil\r\n");
						}

					}
				} else {
					if (writeAccess) {
						PRINTK::PrintK(PRINTK_DEBUG "Write -> Krnl\r\n");
					} else {
						PRINTK::PrintK(PRINTK_DEBUG "Read -> Krnl\r\n");
					}

					// KILL PROCESS
				}

				PANIC("Page fault");
			} else {
				PANIC("Page fault");
			}

			}
			break;
		case 32: {
			Scheduler *scheduler = info->BootDomain->DomainScheduler;
			ThreadControlBlock *activeThread = scheduler->Running;
			SchedulerContext *taskContext = activeThread->Context;

			taskContext->SP = context->Base.IretRSP;
			taskContext->BP = context->Base.IretRSP;
			taskContext->IP = context->Base.IretRIP;
			taskContext->RFLAGS = context->Base.IretRFLAGS;

			Memcpy(&taskContext->Registers, &context->Registers, sizeof(GeneralRegisters));

			activeThread = SCHED::Recalculate(scheduler);
			taskContext = activeThread->Context;

			VMM::LoadVirtualSpace(activeThread->MemorySpace);

			context->Base.IretRSP = taskContext->SP;
			context->Base.IretRSP = taskContext->BP;
			context->Base.IretRIP = taskContext->IP;
			context->Base.IretRFLAGS = taskContext->RFLAGS;

			Memcpy(&context->Registers, &taskContext->Registers, sizeof(GeneralRegisters));

			}
			break;
		default:
			PRINTK::PrintK(PRINTK_DEBUG "Unhandled interrupt: 0x%x\r\n", context->Base.VectorNumber);
			OOPS("Unhandled interrupt");
			break;
	}
	
	return context;
}
}
