#pragma once

#include <cstdint.hpp>

/* Setting the Kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

struct GDTPointer{
	u16 Size;
	u64 Offset;
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

extern "C" void FlushGDT(GDTPointer *pointer);
extern "C" void FlushTSS();

namespace x86_64 {
	void LoadGDT(GDT *gdt, GDTPointer *gdtPointer);

	void LoadNewStackInTSS(TSS *tss, uptr stackPointer);
	void TSSInit(GDT *gdt, TSS *tss, uptr stackPointer);
}
