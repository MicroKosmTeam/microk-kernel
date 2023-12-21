#pragma once
#include <cstdint.hpp>
#include <arch/x86/gdt.hpp>
#include <arch/x86/cpu.hpp>

/* Max number of interrupts in x86_64 is 256 */
#define IDT_MAX_DESCRIPTORS 256

struct IDTEntry {
	u16    ISRLow;      // The lower 16 bits of the ISR's address
	u16    KernelCs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	u8	    IST;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	u8     Attributes;   // Type and attributes; see the IDT page
	u16    ISRMid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	u32    ISRHigh;     // The higher 32 bits of the ISR's address
	u32    Reserved0;     // Set to zero
} __attribute__((packed));

struct IDTR {
	u16 Limit;
#if defined(__x86_64__)
	u64 Base;
#else
	u32 Base;
#endif
} __attribute__((packed));

struct InterruptStatus {
	GeneralRegisters Registers;
	InterruptStack Base;
}__attribute__((packed));

namespace x86 {
	void IDTInit();
	void IDTSetDescriptor(u8 vector, void *isr, u8 ist, u8 flags);
}
