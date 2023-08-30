#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mm/bootmem.hpp>
#include <mm/heap.hpp>
#include <init/kinfo.hpp>

static char *memTypeStrings[] = {
	"Usable",
	"Reserved",
	"ACPI Reclaimable",
	"ACPI NVS",
	"Bad",
	"Bootloader reclaimable",
	"Kernel and modules",
	"Framebuffer"
};

void *Malloc(size_t size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	if(BOOTMEM::BootmemIsActive()) return BOOTMEM::Malloc(size);
	else return NULL;
}

void Free(void *p) {
	HEAP::Free(p);
}

void *operator new(size_t size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	if(BOOTMEM::BootmemIsActive()) return BOOTMEM::Malloc(size);
	else return NULL;
}

void *operator new[](size_t size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	if(BOOTMEM::BootmemIsActive()) return BOOTMEM::Malloc(size);
	else return NULL;
}

void operator delete(void* p) {
	// Now, here comes the problem in deciding who allocated this block
	// We should assume that someone that allocs on BOOTMEM
	// will not call free

	HEAP::Free(p);
}

void operator delete(void* p, size_t size) {
	// Now, here comes the problem in deciding who allocated this block
	// We should assume that someone that allocs on BOOTMEM
	// will not call free

	(void)size;
	HEAP::Free(p);
}

void operator delete[](void* p) {
	// Now, here comes the problem in deciding who allocated this block
	// We should assume that someone that allocs on BOOTMEM
	// will not call free

	HEAP::Free(p);
}

void operator delete[](void* p, size_t size) {
	// Now, here comes the problem in deciding who allocated this block
	// We should assume that someone that allocs on BOOTMEM
	// will not call free

	(void)size;
	HEAP::Free(p);
}

namespace MEM {

void DisplayRam() {
	KInfo *info = GetInfo();

	PRINTK::PrintK("Provided physical RAM map:\r\n");

	for (size_t i = 0; i < info->MemoryMapEntryCount; i++) {
		PRINTK::PrintK(" [0x%x - 0x%x] -> %s\r\n",
				info->MemoryMap[i].base,
				info->MemoryMap[i].base + info->MemoryMap[i].length,
				memTypeStrings[info->MemoryMap[i].type]);
	}

}


void Init() {
	VMM::InitVMM();
}
}
