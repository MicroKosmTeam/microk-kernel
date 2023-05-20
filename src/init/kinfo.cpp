#include <init/kinfo.hpp>
#include <mm/bootmem.hpp>

/* Contains some basic information to be passed between components of the kernel */
KInfo *info = NULL;

/* Allocate the KInfo struct with bootmem 
 * Only executed once
 */
void InitInfo() {
	info = (KInfo*)BOOTMEM::Malloc(sizeof(KInfo) + 1);

	return info;
}

/* Function that retrieves the KInfo struct
 */
KInfo *GetInfo() {
	return info;
}
