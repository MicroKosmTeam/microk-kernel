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
#include <proc/smp.hpp>
#include <sys/user.hpp>
#include <init/main.hpp>
#include <sys/panic.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <init/kinfo.hpp>
#include <sys/atomic.hpp>
#include <sys/printk.hpp>
#include <init/modules.hpp>
#include <dev/acpi/acpi.hpp>
#include <arch/x64/main.hpp>
#include <proc/scheduler.hpp>

/*
   Function that is started by the scheduler once kernel startup is complete.
*/
void RestInit(PROC::Thread *thread);
extern "C" void UserFunction();

int EarlyKernelInit() {
	/* Allocating memory for the info struct */
	InitInfo();

	/* Loading early serial printk */
	PRINTK::EarlyInit();

	return 0;
}

void PrintBanner() {
	/* Getting some useful system info */
	KInfo *info = GetInfo();

	/* Printing banner */
	PRINTK::PrintK(" __  __  _                _  __\r\n"
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
			info->kernelVirtualBase,
			BOOTMEM::GetFree(),
			BOOTMEM::GetTotal(),
			BOOTMEM::GetTotal() - BOOTMEM::GetFree());
}

#include <arch/x64/dev/apic.hpp>

__attribute__((__aligned__((16))))volatile char userStack[8192];
/*
   Main kernel function.
*/
__attribute__((noreturn)) void KernelStart() {
	PRINTK::PrintK("Kernel started.\r\n");

	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator();

#ifdef CONFIG_ARCH_x86_64
	/* Starting x86_64 specific instructions */
	x86_64::Init();
#endif
	/* Starting the memory subsystem */
	MEM::Init();

	/* Initializing the heap */
	HEAP::InitializeHeap(CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE / 0x1000);

	/* With the heap initialized, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();

	/* Initialize the ACPI subsystem */
	ACPI::Init();

#ifdef CONFIG_MP_SMP
	/* Initializing multiprocessing */
	PROC::SMP::Init();
#endif
	/* Initializing the scheduler framework */
	PROC::Scheduler::Initialize();

	/* Finishing kernel startup */
	PROC::Scheduler::StartKernelThread(RestInit);

#ifdef CONFIG_KERNEL_MODULES
	/* Starting the modules subsystem */
	MODULE::Init();
#endif

	/* Starting the kernel scheduler by adding the root CPU */
	PROC::Scheduler::AddCPU();
}

void RestInit(PROC::Thread *thread) {
	/* Printing banner to show off */
	PrintBanner();

	/* We are done with the boot process */
	PRINTK::PrintK("Kernel is now resting...\r\n");

	/* We enable the timer to start task-switching */
	x86_64::SetAPICTimer();
	
	void *stack = &userStack[8191];
	void *func = UserFunction;
	PRINTK::PrintK("Switching to userspace.\r\n"
	               " Function Address: 0x%x\r\n"
		       " Stack Address:    0x%x\r\n",
		       (uint64_t)func,
		       (uint64_t)stack);
	EnterUserspace(func, stack);

	PRINTK::PrintK("Returned from userspace.\r\n");

	while (true) CPUPause();
}

inline size_t __x64_syscall(size_t syscallNum, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
	asm volatile("int $0xFE" 
			: "=S"(arg1) 
			: "D"(syscallNum), "S"(arg1), "a"(arg2), "b"(arg3), "c"(arg4), "d"(arg5));

	return arg1;
}

size_t Syscall(size_t syscallNum, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
	return __x64_syscall(syscallNum, arg1, arg2, arg3, arg4, arg5);
}

#include <debug/stack.hpp>
extern "C" void UserFunction() {
	PRINTK::PrintK("We are in usermode\r\n");
	uint64_t syscalls = 10000000;

	for(uint64_t i = 0; i < syscalls; ++i) {
		Syscall(2, 1, 0, 0, 0, 0);
	}

	PRINTK::PrintK("We have done %d syscalls\r\n", syscalls);

	//ExitUserspace();

	while(true);
}
