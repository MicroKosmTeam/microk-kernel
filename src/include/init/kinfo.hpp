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
#include <stddef.h>
#include <stdint.h>
#include <sys/tables.hpp>
#include <mm/memory.hpp>
#include <sys/file.hpp>
#include <mm/vmm.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>
#include <module/modulemanager.hpp>
#include <module/buffer.hpp>
#include <module/section.hpp>
#include <dev/uart.hpp>

/*
 * struct: Framebuffer 
 * Contains the information the kernel stores for a framebuffer.
 *
 * attributes: packed
 */
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
	MEM::MMapEntry *MemoryMap; /* Pointer to the memory map */
	size_t MemoryMapEntryCount; /* Number of memory map regions */

	uintptr_t HigherHalfMapping; /* Start of higher half mapping */

	uintptr_t KernelPhysicalBase; /* Start of the kernel in physical memory */
	uintptr_t KernelVirtualBase; /* Start of the kernel in virtual memory */

	PROC::Scheduler *KernelScheduler;
	VMM::VirtualSpace *KernelVirtualSpace; /* Kernel virtual memory space */
	PROC::KernelProcess *KernelProcess;
	uintptr_t KernelStack; /* Start of kernel stack */

	VMM::PageList *KernelHeapPageList;

#ifdef CONFIG_KERNEL_MODULES
	/* Module information */
	FILE::BootFile *BootFiles; /* Pointer to the Limine modules */
	size_t FileCount; /* Number of modules provided */

	char UserModuleName[256];

	MODULE::Manager *KernelModuleManager;
	MODULE::BufferManager *KernelBufferManager;
	MODULE::SectionManager *KernelSectionManager;

	uintptr_t KernelBaseSystemTable;
	uintptr_t BootFileSystemTable;
#endif

#ifdef CONFIG_HW_UART
	/* Kernel serial device */
	UARTDevice *KernelPort; /* UART deivice used as serial port */
#endif

	void *RSDP;

	size_t FramebufferCount;
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
void InitInfo();

/*
 * function: GetInfo 
 * This function returns a pointer to the KInfo struct.
 * If the struct wasn't allocated prior with the InitInfo function,
 * it is undefined behavior.
 * 
 * arguments: void 
 * return: KInfo* 
 * The pointer to the KInfo struct.
 */
KInfo *GetInfo();
