#pragma once
#include <stdint.h>
#include <stddef.h>

/* Setting the Kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

typedef struct {
	uint16_t Size;
	uint64_t Offset;
} __attribute__((packed)) GDTPointer;

typedef struct {
	uint16_t Limit;
	uint16_t BaseLow16;
	uint8_t BaseMid8;
	uint8_t Access;
	uint8_t Granularity;
	uint8_t BaseHigh8;
}__attribute__((packed)) GDTEntry;

typedef struct {
	uint32_t Reserved0;
	uint64_t RSP0;
	uint64_t RSP1;
	uint64_t RSP2;
	uint64_t Reserved1;
	uint64_t IST1;
	uint64_t IST2;
	uint64_t IST3;
	uint64_t IST4;
	uint64_t IST5;
	uint64_t IST6;
	uint64_t IST7;
	uint64_t Reserved2;
	uint16_t Reserved3;
	uint16_t IOPBOffset;
} __attribute__((packed)) TSS;

typedef struct  {
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
} __attribute__((packed)) GDT;

	void LoadGDT(GDT *gdt, GDTPointer *gdtPointer);
	uint16_t GetLimit(GDT *gdt, uint8_t desc);
	uintptr_t GetBase(GDT *gdt, uint8_t desc);
	uint8_t GetAccess(GDT *gdt, uint8_t desc);
	uint8_t GetGranularity(GDT *gdt, uint8_t desc);

	void LoadNewStackInTSS(TSS *tss, uintptr_t stackPointer);
	void TSSInit(GDT *gdt, TSS *tss, uintptr_t stackPointer);
