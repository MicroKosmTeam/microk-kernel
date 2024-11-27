#include <arch/x86_64/boot/gdt.h>
#include <string.h>

extern void FlushGDT(GDTPointer *gdt);
extern void FlushTSS();

/*
   Function that initializes the TSS given the current kernel stack
*/
void LoadNewStackInTSS(TSS *tss, uintptr_t stackPointer) {
	/* Initializing the stack pointer */
	tss->RSP0 = stackPointer;
	tss->IST1 = tss->RSP0 - 64 * 1024;
	tss->IST2 = tss->IST1 - 64 * 1024;
	tss->IST3 = tss->IST2 - 64 * 1024;
	tss->IST4 = tss->IST3 - 64 * 1024;
	tss->IST5 = tss->IST4 - 64 * 1024;
	tss->IST6 = tss->IST5 - 64 * 1024;
	tss->IST7 = tss->IST6 - 64 * 1024;

}

void TSSInit(GDT *gdt, TSS *tss, uintptr_t stackPointer) {
	/* Cleaning the TSS struct */
	memclr(tss, sizeof(*tss));

	LoadNewStackInTSS(tss, stackPointer);

	/* Giving TSS size */
	tss->IOPBOffset = sizeof(*tss);

	/* Setting TSS parameters in the GDT */
	uint64_t TSSBase = ((uint64_t)tss);
	gdt->TSSLow.BaseLow16 = TSSBase & 0xffff;
	gdt->TSSLow.BaseMid8 = (TSSBase >> 16) & 0xff;
	gdt->TSSLow.BaseHigh8 = (TSSBase >> 24) & 0xff;
	gdt->TSSLow.Limit = sizeof(*tss);
	gdt->TSSHigh.Limit = (TSSBase >> 32) & 0xffff;
	gdt->TSSHigh.BaseLow16 = (TSSBase >> 48) & 0xffff;
	
	FlushTSS();
}

/*
   Function that loads the GDT to the CPU
*/
void LoadGDT(GDT *gdt, GDTPointer *gdtPointer) {
	memclr(gdt, sizeof(*gdt));
	/* Setting GDT pointer size and offset */
	gdtPointer->Size = sizeof(*gdt) - 1;
	gdtPointer->Offset = (uint64_t)(uintptr_t)gdt;

	gdt->Null = (GDTEntry){0, 0, 0, 0, 0, 0}; /* Required null gdt segment */
	gdt->KernelCode16Bit = (GDTEntry){
		.Limit = 0xffff,
		.BaseLow16 = 0, 
		.BaseMid8 = 0,
		.Access = 0x9a,
		.Granularity = 0x80,
		.BaseHigh8 = 0
	}; /* 16-bit code */
	gdt->KernelData16Bit = (GDTEntry){0xffff, 0, 0, 0x92, 0x80, 0}; /* 16-bit data */
	gdt->KernelCode32Bit = (GDTEntry){0xffff, 0, 0, 0x9a, 0xcf, 0}; /* 32-bit code */
	gdt->KernelData32Bit = (GDTEntry){0xffff, 0, 0, 0x92, 0xcf, 0}; /* 32-bit data */
	gdt->KernelCode64Bit = (GDTEntry){0, 0, 0, 0x9a, 0xa0, 0}; /* 64-bit code */
	gdt->KernelData64Bit = (GDTEntry){0, 0, 0, 0x92, 0xa0, 0}; /* 64-bit data */
	gdt->UserCode = (GDTEntry){0, 0, 0, 0x9a, 0xa0, 0}; /* User code */
	gdt->UserData = (GDTEntry){0, 0, 0, 0x92, 0xa0, 0}; /* User data */
	gdt->TSSLow = (GDTEntry){0, 0, 0, 0x89, 0xa0, 0};  /* TSS low */
	gdt->TSSHigh = (GDTEntry){0, 0, 0, 0x00, 0x00, 0};  /* TSS high */

	/* Call to asm functions that load the GDT and TSS */
	FlushGDT(gdtPointer);
}

uint16_t GetLimit(GDT *gdt, uint8_t desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Limit;
}

uintptr_t GetBase(GDT *gdt, uint8_t desc) {
	return (((GDTEntry*)gdt)[desc/0x8].BaseLow16 |
	       ((GDTEntry*)gdt)[desc/0x8].BaseMid8 << 16 |
	       ((GDTEntry*)gdt)[desc/0x8].BaseHigh8 << 24);
}

uint8_t GetAccess(GDT *gdt, uint8_t desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Access;
}

uint8_t GetGranularity(GDT *gdt, uint8_t desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Granularity;
}

