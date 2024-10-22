#pragma once
#include <cdefs.h>

/* Setting the Kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

struct GDTPointer{
	u16 Size;
#if defined(__x86_64)
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

namespace x86 {
	void LoadGDT(GDT *gdt, GDTPointer *gdtPointer);
	u16 GetLimit(GDT *gdt, u8 desc);
	uptr GetBase(GDT *gdt, u8 desc);
	u8 GetAccess(GDT *gdt, u8 desc);
	u8 GetGranularity(GDT *gdt, u8 desc);

	void LoadNewStackInTSS(TSS *tss, uptr stackPointer);
	void TSSInit(GDT *gdt, TSS *tss, uptr stackPointer);
}
