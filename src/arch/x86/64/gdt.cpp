/*
   File: arch/x64/cpu/gdt.cpp
*/

#include <memory.hpp>
#include <arch/x86/gdt.hpp>
#include <vmm.hpp>

void FlushGDT(GDTPointer *gdt) {
	asm volatile ("lgdt [%0]\n\t"
		      "push 0x28\n\t"
		      "lea %%rax, [%%rip + 1f]\n\t"
		      "push %%rax\n\t"
		      "retfq\n\t"
		      "1:\n\t"
		      "mov ax, 0x30\n\t"
		      "mov ds, ax\n\t"
		      "mov es, ax\n\t"
		      "mov ss, ax\n\t"
		      "mov fs, ax\n\t"
		      "mov gs, ax\n\t"
		      : : "r"(gdt));
}

void FlushTSS() {
	asm volatile ("mov ax, 0x48\n\t"
		      "ltr ax\n\t");
}


namespace x86 {
/*
   Function that initializes the TSS given the current kernel stack
*/
void LoadNewStackInTSS(TSS *tss, uptr stackPointer) {
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

void TSSInit(GDT *gdt, TSS *tss, uptr stackPointer) {
	/* Cleaning the TSS struct */
	Memclr(tss, sizeof(*tss));

	LoadNewStackInTSS(tss, stackPointer);

	/* Giving TSS size */
	tss->IOPBOffset = sizeof(*tss);

	/* Setting TSS parameters in the GDT */
	u64 TSSBase = ((u64)tss);
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
	/* Setting GDT pointer size and offset */
	gdtPointer->Size = sizeof(*gdt) - 1;
	gdtPointer->Offset = (u64)(uptr)gdt;

	gdt->Null = {0, 0, 0, 0, 0, 0}; /* Required null gdt segment */
	gdt->KernelCode16Bit = {0xffff, 0, 0, 0x9a, 0x80, 0}; /* 16-bit code */
	gdt->KernelData16Bit = {0xffff, 0, 0, 0x92, 0x80, 0}; /* 16-bit data */
	gdt->KernelCode32Bit = {0xffff, 0, 0, 0x9a, 0xcf, 0}; /* 32-bit code */
	gdt->KernelData32Bit = {0xffff, 0, 0, 0x92, 0xcf, 0}; /* 32-bit data */
	gdt->KernelCode64Bit = {0, 0, 0, 0x9a, 0xa0, 0}; /* 64-bit code */
	gdt->KernelData64Bit = {0, 0, 0, 0x92, 0xa0, 0}; /* 64-bit data */
	gdt->UserCode = {0, 0, 0, 0x9a, 0xa0, 0}; /* User code */
	gdt->UserData = {0, 0, 0, 0x92, 0xa0, 0}; /* User data */
	gdt->TSSLow = {0, 0, 0, 0x89, 0xa0, 0};  /* TSS low */
	gdt->TSSHigh = {0, 0, 0, 0x00, 0x00, 0};  /* TSS high */

	/* Call to asm functions that load the GDT and TSS */
	FlushGDT(gdtPointer);
}

u16 GetLimit(GDT *gdt, u8 desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Limit;
}

uptr GetBase(GDT *gdt, u8 desc) {
	return (((GDTEntry*)gdt)[desc/0x8].BaseLow16 |
	       ((GDTEntry*)gdt)[desc/0x8].BaseMid8 << 16 |
	       ((GDTEntry*)gdt)[desc/0x8].BaseHigh8 << 24);
}

u8 GetAccess(GDT *gdt, u8 desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Access;
}

u8 GetGranularity(GDT *gdt, u8 desc) {
	return ((GDTEntry*)gdt)[desc/0x8].Granularity;
}

}
