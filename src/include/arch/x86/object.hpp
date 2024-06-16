#pragma once
#include <cdefs.h>

#define PAGE_SIZE 0x1000
#define HUGE_PAGE_SIZE (512 * PAGE_SIZE)
#define HUGER_PAGE_SIZE (512 * HUGE_PAGE_SIZE)

namespace x86 {
	struct InterruptStack {
		u64 VectorNumber;
		u64 ErrorCode;

		u64 IretRIP;
		u64 IretCS;
		u64 IretRFLAGS;
		u64 IretRSP;
		u64 IretSS;
	}__attribute__((packed));

	struct GeneralRegisters {
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
	}__attribute__((packed));

	struct GDTPointer{
		u16 Size;
#if defined(__x86_64__)
		u64 Offset;
#else
		u32 Offset;
#endif
	}__attribute__((packed));

	struct GDTEntry{
		u16 Limit;
		u16 BaseLow16;
		u8 BaseMid8;
		u8 Access;
		u8 Granularity;
		u8 BaseHigh8;
	}__attribute__((packed));

	struct TSS {
#if defined(__x86_64__)
		u32 Reserved0;
		u64 RSP0;
		u64 RSP1;
		u64 RSP2;
		u64 Reserved1;
		u64 IST1;
		u64 IST2;
		u64 IST3;
		u64 IST4;
		u64 IST5;
		u64 IST6;
		u64 IST7;
		u64 Reserved2;
		u16 Reserved3;
		u16 IOPBOffset;
#else
		u16 Link;
		u16 Reserved0;
		u32 ESP0;
		u16 SS0;
		u16 Reserved1;
		u32 ESP1;
		u16 SS1;
		u16 Reserved2;
		u32 ESP2;
		u16 SS2;
		u16 Reserved3;
		u32 CR3;
		u32 EIP;
		u32 EFLAGS;
		u32 EAX;
		u32 ECX;
		u32 EDX;
		u32 EBX;
		u32 ESP;
		u32 EBP;
		u32 ESI;
		u32 EDI;
		u16 ES;
		u16 Reserved4;
		u16 CS;
		u16 Reserved5;
		u16 SS;
		u16 Reserved6;
		u16 DS;
		u16 Reserved7;
		u16 FS;
		u16 Reserved8;
		u16 GS;
		u16 Reserved9;
		u16 LDTR;
		u16 Reserved10;
		u16 Reserved11;
		u16 IOPBOffset;
		u32 SSP;
#endif
	} __attribute__((packed));

	struct GDT {
		GDTEntry Null;

		GDTEntry KernelCode16Bit;
		GDTEntry KernelData16Bit;

		GDTEntry KernelCode32Bit;
		GDTEntry KernelData32Bit;

		GDTEntry KernelCode64Bit;
		GDTEntry KernelData64Bit;

		GDTEntry UserCode;
		GDTEntry UserData;

		GDTEntry TSSLow;
		GDTEntry TSSHigh;
	} __attribute__((packed));


}

