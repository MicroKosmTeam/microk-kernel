/*
   File: arch/x64/cpu/gdt.cpp
*/

/* We are using this to prevent GCC from optimizing this very delicate code */
#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <mm/memory.hpp>
#include <arch/x64/cpu/gdt.hpp>

/* This is the GDT. It must be aligned, otherwise it won't work.
   This follows the recomended GDT by Limine */
__attribute__((aligned(0x1000))) GDT gdt = {
	{0, 0, 0, 0, 0, 0}, /* Required null gdt segment */
	{0xffff, 0, 0, 0x9a, 0x80, 0}, /* 16-bit code */
	{0xffff, 0, 0, 0x92, 0x80, 0}, /* 16-bit data */
	{0xffff, 0, 0, 0x9a, 0xcf, 0}, /* 32-bit code */
	{0xffff, 0, 0, 0x92, 0xcf, 0}, /* 32-bit data */
	{0, 0, 0, 0x9a, 0xa0, 0}, /* 64-bit code */
	{0, 0, 0, 0x92, 0xa0, 0}, /* 64-bit data */
	{0, 0, 0, 0x9a, 0xa0, 0}, /* User code */
	{0, 0, 0, 0x92, 0xa0, 0}, /* User data */
	{0, 0, 0, 0x89, 0xa0, 0},  /* TSS low */
	{0, 0, 0, 0x00, 0x00, 0},  /* TSS high */
};

/* Aligned GDT Pointer */
__attribute__((aligned(0x1000))) GDTPointer gdtPointer;
/* The TSS must be aligned */
__attribute__((aligned(0x1000))) TSS tss;

namespace x86_64 {
/*
   Function that initializes the TSS given the current kernel stack
*/
void TSSInit(uintptr_t stackPointer) {
	/* Cleaning the TSS struct */
	memset(&tss, 0, sizeof(tss));

	/* Initializing the stack pointer */
	tss.rsp0 = stackPointer;
	tss.ist1 = 0;
	/* Giving TSS size */
	tss.iopb_offset = sizeof(tss);

	/* Setting TSS parameters in the GDT */
	uint64_t TSSBase = ((uint64_t)&tss);
	gdt.tss_low.base_low16 = TSSBase & 0xffff;
	gdt.tss_low.base_mid8 = (TSSBase >> 16) & 0xff;
	gdt.tss_low.base_high8 = (TSSBase >> 24) & 0xff;
	gdt.tss_low.limit = sizeof(tss);
	gdt.tss_high.limit = (TSSBase >> 32) & 0xffff;
	gdt.tss_high.base_low16 = (TSSBase >> 48) & 0xffff;
}

/*
   Function that loads the GDT to the CPU
*/
void LoadGDT(uintptr_t stackPointer) {
	/* Initialization of the TSS */
	TSSInit(stackPointer);

	/* Setting GDT pointer size and offset */
	gdtPointer.size = sizeof(gdt) - 1;
	gdtPointer.offset = (uint64_t)&gdt;

	/* Call to asm functions that load the GDT and TSS */
	FlushGDT(&gdtPointer);
	FlushTSS();
}
}

#pragma GCC pop_options
