/*
   File: include/init/kinfo.hpp
*/

#pragma once
#include <cdefs.h>
#include <stddef.h>
#include <stdint.h>
#include <mm/vmm.hpp>
#include <proc/process.hpp>

#ifdef CONFIG_HW_UART
#include <dev/uart/uart.hpp>
#endif

#define MEMMAP_USABLE                 0
#define MEMMAP_RESERVED               1
#define MEMMAP_ACPI_RECLAIMABLE       2
#define MEMMAP_ACPI_NVS               3
#define MEMMAP_BAD_MEMORY             4
#define MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define MEMMAP_KERNEL_AND_MODULES     6
#define MEMMAP_FRAMEBUFFER            7

struct MMapEntry {
	uint64_t base;
	uint64_t length;
	uint64_t type;
};

#ifdef CONFIG_KERNEL_MODULES
struct BootFile {
	void *address;
	uint64_t size;
	char *path;
	char *cmdline;
};
#endif

/*
   KInfo struct
    Contains some basic information to be passes between components of the kernel
    Sould only be allocated once by the respective bootloader function.
*/
struct KInfo {
	/* Memory information */
	MMapEntry *mMap; /* Pointer to the memory map */
	uint64_t mMapEntryCount; /* Number of memory map regions */

	uintptr_t higherHalfMapping; /* Start of higher half mapping */
	uintptr_t kernelStack; /* Start of kernel stack */

	uintptr_t kernelPhysicalBase; /* Start of the kernel in physical memory */
	uintptr_t kernelVirtualBase; /* Start of the kernel in virtual memory */

	VMM::VirtualSpace *kernelVirtualSpace; /* Kernel virtual memory space */
	PROC::Process *kernelProcess;

#ifdef CONFIG_KERNEL_MODULES
	/* Module information */
	BootFile *modules; /* Pointer to the Limine modules */
	uint64_t moduleCount; /* Number of modules provided */
#endif

#ifdef CONFIG_HW_UART
	/* Kernel serial device */
	UARTDevice *kernelPort; /* UART deivice used as serial port */
#endif

	void *RSDP;
};

void InitInfo();
KInfo *GetInfo();
