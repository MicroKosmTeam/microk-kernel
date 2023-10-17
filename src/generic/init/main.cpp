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
 * File: generic/init/main.cpp
 */

#include <cdefs.h>
#include <cstdint.hpp>

#include <mm/pmm.hpp>
#include <mm/heap.hpp>
#include <init/main.hpp>
#include <init/kargs.hpp>
#include <sys/panic.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <init/kinfo.hpp>
#include <sys/atomic.hpp>
#include <sys/printk.hpp>
#include <sys/loader.hpp>
#include <sys/syscall.hpp>
#include <proc/scheduler.hpp>

#if defined(ARCH_x64)
#include <arch/x64/main.hpp>
#elif defined(ARCH_aarch64)
#include <arch/aarch64/main.hpp>
#endif


int EarlyKernelInit() {
	/* Allocating memory for the info struct */
	InitInfo();

	/* Initializing early architecture-specific devices */
#if defined(ARCH_x64)
	x86_64::EarlyInit();
#elif defined(ARCH_aarch64)
	AArch64::EarlyInit();
#endif

	/* Loading early serial printk */
	PRINTK::EarlyInit();

	return 0;
}


void PrintBanner() {
	/* Getting some useful system info */
	KInfo *info = GetInfo();

	/* Printing banner */
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " __  __  _                _  __\r\n"
		       "|  \\/  |(_) __  _ _  ___ | |/ /\r\n"
		       "| |\\/| || |/ _|| '_|/ _ \\|   < \r\n"
		       "|_|  |_||_|\\__||_|  \\___/|_|\\_\\\r\n"
		       "The operating system for the future...at your fingertips.\r\n"
		       "%s %s Started.\r\n"
		       "System stats:\r\n"
		       "  Memory:\r\n"
		       "   Physical: %dkb free out of %dkb (%dkb used).\r\n"
		       "   Virtual: Kernel at virtual address 0x%x.\r\n"
		       "   Bootmem: %d bytes free out of %d bytes (%d bytes used).\r\n",
		       CONFIG_KERNEL_CNAME,
		       CONFIG_KERNEL_CVER,
			PMM::GetFreeMem() / 1024,
			(PMM::GetFreeMem() + PMM::GetUsedMem()) / 1024,
			PMM::GetUsedMem() / 1024,
			info->KernelVirtualBase,
			BOOTMEM::GetFree(),
			BOOTMEM::GetTotal(),
			BOOTMEM::GetTotal() - BOOTMEM::GetFree());
}

void RestInit() {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "The kernel is resting.\r\n");
	
	while(true) {
		asm volatile("hlt");
	}
}

#include <arch/x64/dev/apic.hpp>
__attribute__((noreturn)) void KernelStart() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "MicroKosm is booted. Cmdline: %s\r\n", info->KernelArgs);
	ParseArgs();

	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator();

	/* Starting the memory subsystem */
	MEM::DisplayRam();
	MEM::Init();

	/* Initializing the heap */
	HEAP::InitializeHeap((void*)CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE / PAGE_SIZE);

	/* With the heap initialized, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();

	/* Starting architecture-specific instructions */
#if defined(ARCH_x64)
	x86_64::Init();
#elif defined(ARCH_aarch64)
	AArch64::Init();
#endif

	/* Printing banner to show off */
	PrintBanner();

#ifdef CONFIG_KERNEL_MODULES
	/* Initializing the scheduler framework */
	info->KernelScheduler = PROC::InitializeScheduler(SCHEDULER_DEFAULT_QUEUES);
	InitSyscalls();
	InitializeKernelTables();
/*
	info->KernelProcess = (PROC::KernelProcess*)PROC::CreateProcess((PROC::ProcessBase*)info->KernelProcess, PROC::ExecutableUnitType::PT_KERNEL, info->KernelVirtualSpace, 0, 0);
	PROC::KernelThread *kernelThread = (PROC::KernelThread*)PROC::CreateThread((PROC::ProcessBase*)info->KernelProcess, (uptr)&RestInit, 64 * 1024, 0, 0);
	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, kernelThread);
*/
	/* Load all the files we are told to load */
	usize moduleSize;
	u8 *addr;

	/* Get the memory location of the user module */
	addr = (u8*)FindFile(info->UserModuleName, &moduleSize);
	
	/* Check if it is present */
	if (addr != NULL) {
		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Loading user module from 0x%x\r\n", addr);

		/* Create the process and set it to a ready state */
		usize pid = LoadExecutableFile(addr, moduleSize);
		PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, pid, 0), PROC::ExecutableUnitState::P_READY);

		/* Recalculate the scheduler and wait for the context switch */
		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Switching to user module.\r\n");
#if defined(ARCH_x64)
		x86_64::WaitAPIC();
#endif
	} else PANIC("Could not find User Module");
#endif

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Kernel startup complete.\r\n");

	/* We are done */
	while (true) CPUPause();
}
