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

#include <dev/cpu.hpp>
#include <sys/arch.hpp>
#include <mm/memory.hpp>
#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <init/kargs.hpp>
#include <sys/loader.hpp>
#include <sys/syscall.hpp>

extern "C" void CPUPause();

extern "C" __attribute__((noreturn))
void KernelStart() {
	KInfo *info = GetInfo();

	/* Loading early serial printk */
	PRINTK::EarlyInit();

	DEV::CPU::InitializeBootCPU();

	/* Parsing cmdline arguments */
	ParseArgs();

	/* Starting the memory subsystem */
	MEM::Init();

	/* Starting architecture-specific instructions */
	ARCH::SetupArch();

	info->KernelScheduler = PROC::InitializeScheduler(SCHEDULER_DEFAULT_QUEUES);
	info->KernelMessageManager = PROC::IPCMessageManagerInitialize();

	PROC::IPCMessageQueueCtl(info->KernelMessageManager, PROC::QueueOperations::CREATE, NULL, 4096);
	PROC::IPCMessageQueueCtl(info->KernelMessageManager, PROC::QueueOperations::CREATE, NULL, 4096);

	u8 stringBuffer[32] = {'\0'};

	PRINTK::PrintK(PRINTK_DEBUG "Sent %d bytes to queue 0\r\n", PROC::IPCMessageSend(info->KernelMessageManager, 0, (const u8*)"Hello, Marta!\r\n", 16, 0, 0));
	PRINTK::PrintK(PRINTK_DEBUG "Sent %d bytes to queue 1\r\n", PROC::IPCMessageSend(info->KernelMessageManager, 1, (const u8*)"Hello, Becky!\r\n", 16, 0, 0));
	
	PROC::IPCMessageReceive(info->KernelMessageManager, 1, stringBuffer, 32, 0, 0);
	PRINTK::PrintK(PRINTK_DEBUG "Buffer 1: %s\r\n", stringBuffer);
	PROC::IPCMessageReceive(info->KernelMessageManager, 0, stringBuffer, 32, 0, 0);
	PRINTK::PrintK(PRINTK_DEBUG "Buffer 0: %s\r\n", stringBuffer);

	InitSyscalls();
	InitializeKernelTables();

#ifdef CONFIG_KERNEL_MODULES
	usize moduleSize;
	u8 *addr;

	addr = (u8*)FindFile(info->UserModuleName, &moduleSize);
	
	if (addr != NULL) {
		PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Loading user module from 0x%x\r\n", addr);

		usize pid = LoadExecutableFile(addr, moduleSize);
		PROC::UserThread *thread = (PROC::UserThread*)PROC::GetThread(info->KernelScheduler, pid, 0);
		PROC::SetExecutableUnitState(thread, PROC::ExecutableUnitState::P_READY);

		uptr space = ((PROC::UserProcess*)(thread->Parent))->VirtualMemorySpace;
		PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Switching to user module.\r\n");

		PROC::RecalculateScheduler(info->KernelScheduler);
	
		x86_64::PerCoreCPUTopology *coreInfo = (x86_64::PerCoreCPUTopology*)info->BootCore->ArchitectureSpecificInformation; 
		UpdateLocalCPUStruct(&coreInfo->CPUStruct, thread->KernelStack, VMM::VirtualToPhysical(space), VMM::VirtualToPhysical(space));
		
		VMM::LoadVirtualSpace(space);
		EnterUserspace((void*)thread->Context->IretRIP, (void*)thread->Context->IretRSP);
	} else PANIC("Could not find User Module");
#endif

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Kernel startup complete.\r\n");

	/* We are done */
	while (true) CPUPause();
}
