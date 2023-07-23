/*
   File: include/init/kinfo.hpp
*/

#pragma once
#include <cdefs.h>
#include <stddef.h>
#include <stdint.h>
#include <mm/memory.hpp>
#include <sys/file.hpp>
#include <mm/vmm.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <module/modulemanager.hpp>
#include <module/buffer.hpp>
#include <module/section.hpp>
#include <dev/uart/uart.hpp>

struct Framebuffer {
	void *Address;
	uint32_t Width;
	uint32_t Height;
	uint16_t BPP;
	uint8_t RedShift;
	uint8_t GreenShift;
	uint8_t BlueShift;
}__attribute__((packed));

/*
   KInfo struct
    Contains some basic information to be passes between components of the kernel
    Sould only be allocated once by the respective bootloader function.
*/
struct KInfo {
	/* Kernel args */
	const char *KernelArgs;

	/* Memory information */
	MEM::MMapEntry *mMap; /* Pointer to the memory map */
	uint64_t mMapEntryCount; /* Number of memory map regions */

	uintptr_t higherHalfMapping; /* Start of higher half mapping */
	uintptr_t kernelStack; /* Start of kernel stack */

	uintptr_t kernelPhysicalBase; /* Start of the kernel in physical memory */
	uintptr_t kernelVirtualBase; /* Start of the kernel in virtual memory */

	VMM::VirtualSpace *kernelVirtualSpace; /* Kernel virtual memory space */
	PROC::Process *kernelProcess;
	PROC::Scheduler *kernelScheduler;

#ifdef CONFIG_KERNEL_MODULES
	/* Module information */
	FILE::BootFile *bootFiles; /* Pointer to the Limine modules */
	uint64_t fileCount; /* Number of modules provided */

	MODULE::Manager *KernelModuleManager;
	MODULE::BufferManager *KernelBufferManager;
	MODULE::SectionManager *KernelSectionManager;
#endif

#ifdef CONFIG_HW_UART
	/* Kernel serial device */
	UARTDevice *kernelPort; /* UART deivice used as serial port */
#endif

	void *RSDP;

	size_t framebufferCount;
	Framebuffer *framebuffers;
};

struct KInfo;

void InitInfo();
KInfo *GetInfo();
