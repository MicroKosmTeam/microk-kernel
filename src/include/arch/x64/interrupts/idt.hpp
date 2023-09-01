#pragma once
#include <stdint.h>

/* Setting the Kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

struct IDTEntry {
	uint16_t    ISRLow;      // The lower 16 bits of the ISR's address
	uint16_t    KernelCs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    IST;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     Attributes;   // Type and attributes; see the IDT page
	uint16_t    ISRMid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    ISRHigh;     // The higher 32 bits of the ISR's address
	uint32_t    Reserved0;     // Set to zero
} __attribute__((packed));

struct IDTR {
	uint16_t	Limit;
	uint64_t	Base;
} __attribute__((packed));

struct CPUStatus {
    uint64_t R15;
    uint64_t R14;
    uint64_t R13;
    uint64_t R12;
    uint64_t R11;
    uint64_t R10;
    uint64_t R9;
    uint64_t R8;

    uint64_t RDX;
    uint64_t RCX;
    uint64_t RBX;
    uint64_t RAX;

    uint64_t RSI;
    uint64_t RDI;

    uint64_t VectorNumber;
    uint64_t ErrorCode;
    
    uint64_t IretRIP;
    uint64_t IretCS;
    uint64_t IretRFLAGS;
    uint64_t IretRSP;
    uint64_t IretSS;
} __attribute__((packed));

namespace x86_64 {
	static inline uintmax_t IRQDisable() {
		uintmax_t flags;
		asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
		return flags;
	}

	static inline void IRQRestore(uintmax_t flags) {
		asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
	}

	void IDTInit();
}
