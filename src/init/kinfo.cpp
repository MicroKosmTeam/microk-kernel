#include <init/kinfo.hpp>
#include <mm/bootmem.hpp>

/* Contains some basic information to be passed between components of the kernel */
KInfo *info = NULL;

void InitInfo() {
	info = (KInfo*)BOOTMEM::Malloc(sizeof(KInfo) + 1);

	return info;
}

KInfo *GetInfo() {
	return info;
}
