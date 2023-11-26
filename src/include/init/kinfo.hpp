/*  __  __  _                _  __        ___   ___
 * |  \/  |(_) __  _ _  ___ | |/ /       / _ \ / __|
 * | |\/| || |/ _|| '_|/ _ \|   <       | (_) |\__ \
 * |_|  |_||_|\__||_|  \___/|_|\_\       \___/ |___/
 *
 * MicroKernel, a simple futiristic Unix-shattering kernel
 * Copyright (C) 2022-2023 Mutta Filippo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General does return to userspacePublic License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: include/init/kinfo.hpp
 */

#pragma once
#include <cdefs.h>

#include <cstdint.hpp>
#include <sys/tables.hpp>
#include <mm/memory.hpp>
#include <mm/slab.hpp>
#include <mm/heap.hpp>
#include <mm/memblock.hpp>
#include <sys/file.hpp>
#include <proc/process.hpp>
#include <proc/message.hpp>
#include <dev/cpu.hpp>
#include <proc/scheduler.hpp>
#include <dev/earlycon.hpp>

/*
 * struct: Framebuffer 
 * Contains the information the kernel stores for a framebuffer.
 *
 * attributes: packed
 */
struct Framebuffer {
	void *Address;
	u32 Width;
	u32 Height;
	u16 BPP;
	u8 RedShift;
	u8 GreenShift;
	u8 BlueShift;
}__attribute__((packed));

/*
 * struct: KInfo
 * Contains various data structures that need to be accessible to various
 * kernel components and subsystems.
 *
 * attributes: none
 */
struct KInfo {
	/* Kernel args */
	const char *KernelArgs;

	/* Memory information */
	MEM::MEMBLOCK::MemblockAllocator *PhysicalMemoryChunks; /* Physical memory map */
	MEM::SLAB::SlabAllocator *KernelSlabAllocator;          /* Slab allocator */
	MEM::HEAP::Heap *KernelHeap;                            /* Generic heap allocator */

	uptr HigherHalfMapping; /* Start of higher half mapping */
	uptr KernelPhysicalBase; /* Start of the kernel in physical memory */
	uptr KernelVirtualBase; /* Start of the kernel in virtual memory */


	DEV::CPU::TopologyStructure *DefaultMachine;
	DEV::CPU::TopologyStructure *BootCore;

	PROC::MessageManager *KernelMessageManager;
	PROC::Scheduler *KernelScheduler;
	PROC::KernelProcess *KernelProcess;
	uptr KernelVirtualSpace; /* Kernel virtual memory space */
	uptr KernelStack; /* Start of kernel stack */

	VMM::PageList *KernelHeapPageList;

#ifdef CONFIG_KERNEL_MODULES
	/* Module information */
	BootFile *BootFiles; /* Pointer to the Limine modules */
	usize FileCount; /* Number of modules provided */

	char UserModuleName[MAX_FILE_NAME_LENGTH];

	uptr KernelBaseSystemTable;
	uptr BootFileSystemTable;
#endif

#ifdef CONFIG_HW_UART
	/* Kernel serial device */
	DEV::UART::UARTDevice *KernelPort; /* UART deivice used as serial port */
#endif

	uptr RSDP;
	uptr DeviceTree;

	usize FramebufferCount;
	Framebuffer *Framebuffers;
};

/*
 * function: InitInfo 
 * This function allocates memory for the KInfo struct and
 * clears it to zero. It has to be called before any call to
 * the GetInfo function.
 * 
 * arguments: void 
 * return: void 
 */
inline void InitInfo() {
	extern KInfo KernelInfo;
	Memset(&KernelInfo, 0, sizeof(KInfo));
}

/*
 * function: GetInfo 
 * This function returns a pointer to the KInfo struct.
 * If the InitInfo functio wasn't called prior to this
 * it is undefined behavior.
 * 
 * arguments: void 
 * return: KInfo* 
 * The pointer to the KInfo struct.
 */
inline KInfo *GetInfo() {
	extern KInfo KernelInfo;
	return &KernelInfo;
}
