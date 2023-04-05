/*
   File: init/main.cpp
   __  __  _                _  __        ___   ___
  |  \/  |(_) __  _ _  ___ | |/ /       / _ \ / __|
  | |\/| || |/ _|| '_|/ _ \|   <       | (_) |\__ \
  |_|  |_||_|\__||_|  \___/|_|\_\       \___/ |___/

  MicroKernel, a simple futiristic Unix-shattering kernel
  Copyright (C) 2022-2023 Mutta Filippo

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.hpp>
#include <mm/heap.hpp>
#include <init/main.hpp>
#include <sys/panic.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <init/modules.hpp>
#include <arch/x64/main.hpp>

/*
   Function that is started by the schedduler once kernel startup is complete.
*/
void RestInit();

extern "C" void EnterUserspace(void *function, void *stack);

int EarlyKernelInit() {
	/* Allocating memory for the info struct */
	InitInfo();

	/* Loading early serial printk */
	PRINTK::EarlyInit();

	return 0;
}

/*
   Main kernel function.
*/
void KernelStart() {
	PRINTK::PrintK("Kernel started.\r\n");

	/* Starting specific arch-dependent instructions */

#ifdef CONFIG_ARCH_x86_64
	/* Starting x8_64 specific instructions */
	x86_64::Init();
#endif

	/* Starting the memory subsystem */
	MEM::Init();

	/* Initializing the heap */
	HEAP::InitializeHeap(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE / 0x1000);
	/* With the heap initialize, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();
	PRINTK::PrintK("Free bootmem memory: %dkb out of %dkb.\r\n", BOOTMEM::GetFree() / 1024, BOOTMEM::GetTotal() / 1024);

	/* Printing banner */
	PRINTK::PrintK(" __  __  _                _  __\r\n"
		       "|  \\/  |(_) __  _ _  ___ | |/ /\r\n"
		       "| |\\/| || |/ _|| '_|/ _ \\|   < \r\n"
		       "|_|  |_||_|\\__||_|  \\___/|_|\\_\\\r\n"
		       "The operating system for the future...at your fingertips.\r\n"
		       "%s %s Started.\r\n"
		       "Free memory: %dMB out of %dMB (%d%% free).\r\n",
		       CONFIG_KERNEL_CNAME,
		       CONFIG_KERNEL_CVER,
			PMM::GetFreeMem() / 1024 / 1024,
			(PMM::GetFreeMem() + PMM::GetUsedMem()) / 1024 / 1024,
			(PMM::GetFreeMem() + PMM::GetUsedMem()) / PMM::GetFreeMem() * 100);

	/* Starting the modules subsystem */
	MODULE::Init();

	/* Finishing kernel startup */
	RestInit();
}

extern "C" void UserFunction() {
	for (;;);
}

void RestInit() {
	/* We are done with the boot process */
	PRINTK::PrintK("Kernel is now resting...\r\n");

	void *stack = PMM::RequestPage();
	void *func = &UserFunction; //- GetInfo()->kernelVirtualBase + GetInfo()->kernelPhysicalBase;
	PRINTK::PrintK("Switching to userspace.\r\n"
	               " Function Address: 0x%x\r\n"
		       " Stack Address:    0x%x\r\n",
		       (uint64_t)func,
		       (uint64_t)stack);

	EnterUserspace(func, stack);

	while (true) {
		asm volatile ("hlt");
	}
}
