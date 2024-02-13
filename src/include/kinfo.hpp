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
#include <object.hpp>
#include <memory.hpp>
#include <vmm.hpp>
#include <memblock.hpp>

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

	uptr HigherHalfMapping; /* Start of higher half mapping */
	uptr KernelPhysicalBase; /* Start of the kernel in physical memory */
	uptr KernelVirtualBase; /* Start of the kernel in virtual memory */

	VirtualSpace KernelVirtualSpace; /* Kernel virtual memory space */
	uptr KernelStack; /* Start of kernel stack */

	uptr RSDP;
	uptr DeviceTree;

	uptr ManagerExecutableAddress;
	usize ManagerExecutableSize;

	ThreadControlBlock *RootTCB;
	CapabilitySpace *RootCSpace;

	Domain *BootDomain;
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
	Memclr(&KernelInfo, sizeof(KInfo));
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
