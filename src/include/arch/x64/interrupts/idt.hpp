#pragma once
#include <cstdint.hpp>
#include <arch/x64/cpu/gdt.hpp>

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
	u16	Limit;
	u64	Base;
} __attribute__((packed));

struct CPUStatus {
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

	u64 VectorNumber;
	u64 ErrorCode;

	u64 IretRIP;
	u64 IretCS;
	u64 IretRFLAGS;
	u64 IretRSP;
	u64 IretSS;
} __attribute__((packed));

namespace x86_64 {
	static inline umax IRQDisable() {
		umax flags;
		asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
		return flags;
	}

	static inline void IRQRestore(umax flags) {
		asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
	}

	void IDTInit();
}
