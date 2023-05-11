#pragma once
#include <stdint.h>

struct IDTEntry {
	uint16_t    isrLow;      // The lower 16 bits of the ISR's address
	uint16_t    kernelCs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isrMid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isrHigh;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed));

struct IDTR {
	uint16_t	limit;
	uint64_t	base;
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
	void IDTInit();
}
