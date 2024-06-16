#pragma once
#include <cdefs.h>
#include <arch/x86/object.hpp>

/* Setting the Kernel offset in the GDT (5th entry) */
#define GDT_OFFSET_KERNEL_CODE (0x08 * 5)
#define GDT_OFFSET_USER_CODE (0x08 * 7)

namespace x86 {
	void LoadGDT(GDT *gdt, GDTPointer *gdtPointer);

	void LoadNewStackInTSS(TSS *tss, uptr stackPointer);
	void TSSInit(GDT *gdt, TSS *tss, uptr stackPointer);
}
