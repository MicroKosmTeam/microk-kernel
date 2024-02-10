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
#include <memory.hpp>
#include <panic.hpp>
#include <printk.hpp>
#include <kinfo.hpp>
#include <loader.hpp>
#include <cpu.hpp>
#include <pmm.hpp>
#include <bootmem.hpp>
#include <sched.hpp>
#include <capability.hpp>

extern "C" __attribute__((noreturn))
void KernelStart() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG "Structs size: \r\n"
		       "Capability:         0x%x\r\n"
		       "CapabilityNode:     0x%x\r\n"
		       "CapabilitySpace:    0x%x\r\n"
		       "Domain:             0x%x\r\n"
		       "Scheduler:          0x%x\r\n"
		       "ThreadControlBlock: 0x%x\r\n"
		       "SchedulerContext:   0x%x\r\n",
		       sizeof(Capability),
		       sizeof(CapabilityNode),
		       sizeof(CapabilitySpace),
		       sizeof(Domain),
		       sizeof(Scheduler),
		       sizeof(ThreadControlBlock),
		       sizeof(SchedulerContext));

	ARCH::InitializeBootCPU();
	MEM::Init();
	CAPABILITY::InitializeRootSpace();
	ARCH::InitializeCPUFeatures();
	SCHED::InitializeCPUScheduler(info->BootDomain, VMM::PhysicalToVirtual((uptr)PMM::RequestPage()));
	CAPABILITY::Originate(info->RootCapabilitySpace, (uptr)info->BootDomain->DomainScheduler, sizeof(Scheduler), ObjectType::SCHEDULER, CapabilityRights::NONE);
	LOADER::LoadELF((u8*)info->ManagerExecutableAddress, info->ManagerExecutableSize);
	MEM::Deinit();
	SCHED::Recalculate(info->BootDomain->DomainScheduler);

	PRINTK::PrintK(PRINTK_DEBUG "Kernel startup complete.\r\n");

	VMM::LoadVirtualSpace(info->RootTCB->MemorySpace);
	ARCH::LoadSchedulerContext(info->RootTCB->Context);

	HALT;

	__builtin_unreachable();
}
