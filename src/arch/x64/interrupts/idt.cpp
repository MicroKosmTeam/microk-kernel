/*
   File: arch/x64/interrupts/idt.cpp
*/

#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <sys/user.hpp>
#include <sys/syscall.hpp>
#include <sys/locks.hpp>
#include <proc/helpers.hpp>
#include <arch/x64/interrupts/idt.hpp>
#include <arch/x64/cpu/cpu.hpp>


/* Function to set a descriptor in the GDT */
static void IDTSetDescriptor(IDTEntry *idt, u8 vector, void *isr, u8 ist, u8 flags) {
	/* Create new descriptor */
	volatile IDTEntry *descriptor = &idt[vector];

	/* Setting parameters */
	descriptor->ISRLow = (u64)isr & 0xFFFF;
	descriptor->KernelCs = GDT_OFFSET_KERNEL_CODE;
	descriptor->IST = ist;
	descriptor->Attributes = flags;
	descriptor->ISRMid = ((u64)isr >> 16) & 0xFFFF;
	descriptor->ISRHigh = ((u64)isr >> 32) & 0xFFFFFFFF;
	descriptor->Reserved0 = 0;
}

/* ISR stub table, declared in assembly code */
extern void *isrStubTable[];

namespace x86_64 {
/* Function to initialize the IDT */
void IDTInit(IDTEntry *idt, IDTR *idtr) {
	/* Set base and size in the pointer */
	idtr->Base = (uptr)&idt[0];
	idtr->Limit = (u16)sizeof(IDTEntry) * IDT_MAX_DESCRIPTORS - 1;

	/* Fill in the 32 exception handlers */
	for (u8 vector = 0; vector < 32; vector++) {
		IDTSetDescriptor(idt, vector, isrStubTable[vector], 0, 0x8E);
	}
	
	IDTSetDescriptor(idt, 14, isrStubTable[14], 1, 0x8E);
	IDTSetDescriptor(idt, 32, isrStubTable[32], 2, 0x8F);
	IDTSetDescriptor(idt, 254, isrStubTable[254], 3, 0xEF);

	/* Load the new IDT */
	asm volatile ("lidt %0" : : "m"(*idtr));
	/* Set the interrupt flag */
	asm volatile ("sti");
}
}

static inline void PrintRegs(CPUStatus *context) {
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " -> RAX: 0x%x\r\n"
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

extern "C" CPUStatus *InterruptHandler(CPUStatus *context) {
	KInfo *info = GetInfo();

	DEV::CPU::TopologyStructure *core;
	x86_64::GetCoreTopologyStruct(&core);
	x86_64::PerCoreCPUTopology *coreInfo = (x86_64::PerCoreCPUTopology*)core->ArchitectureSpecificInformation;

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Core topology structure: 0x%x\r\n", core);
	switch(context->VectorNumber) {
		case 0:
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Division by zero.\r\n");
			break;
		case 6:
			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Invalid opcode: 0x%x.\r\n", *(u8*)context->IretRIP);
			break;
		case 8:
			PANIC("Double fault");
			break;
		case 13: {
			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "General protection fault.\r\n");
			PANIC("General protection fault");
			break;
			}
		case 14: {
			uptr page;
			bool protectionViolation = context->ErrorCode & 0b1;
			bool writeAccess = (context->ErrorCode & 0b10) >> 1;
			bool byUser = (context->ErrorCode & 0b100) >> 2;
			bool wasInstructionFetch = (context->ErrorCode & 0b1000) >> 4;
			asm("mov %%cr2,%0" : "=r"(page));

			PrintRegs(context);
			PRINTK::PrintK(PRINTK_DEBUG
				       "Page fault in page 0x%x because of a %s.\r\n"
				       "It was caused by a %s from %s.\r\n"
				       "It %s because of an instruction fetch.\r\n",
				       page,
				       protectionViolation ? "page protection violation" : "non-present page",
				       writeAccess ? "write" : "read",
				       byUser ? "userspace" : "Kernelspace",
				       wasInstructionFetch ? "was" : "wasn't");

			PANIC("Page fault");
			}
			break;
		case 32: {
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "APIC ticked!!\r\n");
			x86_64::APIC::APIC *apic = coreInfo->LocalAPIC;
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "APIC at 0x%x\r\n", apic);

			if(info->KernelScheduler != NULL) {
				if(info->KernelScheduler->CurrentThread != NULL) {
					Memcpy(info->KernelScheduler->CurrentThread->Thread->Context, context, sizeof(CPUStatus));
				}

				if(context->IretCS != GDT_OFFSET_KERNEL_CODE) { 
					context->IretCS = GDT_OFFSET_USER_CODE;
					context->IretSS = GDT_OFFSET_USER_CODE + 0x08;
				}

				info->KernelScheduler->ElapsedQuantum += 1;
				if(PROC::RecalculateScheduler(info->KernelScheduler) == -1) PANIC("Recalculating the scheduler failed");
				
				if(info->KernelScheduler->CurrentThread == NULL) PANIC("AAAA");
				
				PROC::ProcessBase *proc = PROC::GetProcess();

				Memcpy(context, info->KernelScheduler->CurrentThread->Thread->Context, sizeof(CPUStatus));

				x86_64::UpdateLocalCPUStruct(&coreInfo->CPUStruct,
						             info->KernelScheduler->CurrentThread->Thread->KernelStack,
						             VMM::VirtualToPhysical(proc->VirtualMemorySpace->VirtualHierarchyTop),
							     VMM::VirtualToPhysical(proc->VirtualMemorySpace->VirtualHierarchyTop));

				//x86_64::APIC::WriteAPIC(apic, APIC_REGISTER_EOI, 0);

				//u64 tsc = __builtin_ia32_rdtsc() + 0x1000000;
				//x86_64::SetMSR(MSR_TSC_DEADLINE, tsc & 0xFFFFFFFF, tsc >> 32);

				VMM::LoadVirtualSpace(proc->VirtualMemorySpace);
				
				UpdateKernelTables();
			}
			}
			break;
		case 254:
			HandleSyscall(context->RAX, context->RDI, context->RSI, context->RDX, context->RCX, context->R8, context->R9);
			break;
		default:
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Unhandled interrupt: 0x%x\r\n", context->VectorNumber);
			OOPS("Unhandled interrupt");
			break;
	}
	
	return context;
}
