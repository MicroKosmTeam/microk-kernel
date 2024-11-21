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
#include <memory.hpp>
#include <panic.hpp>
#include <printk.hpp>
#include <kinfo.hpp>
#include <loader.hpp>
#include <cpu.hpp>
#include <capability.hpp>
#include <main.hpp>
#include <pmm.hpp>

extern "C" __attribute__((noreturn))
void KernelStart() {
	/* Initializing virtual memory */
	VMM::InitVMM();

	//MEM::Init();
	ARCH::InitializeCPUFeatures();


	UserStart();
	__builtin_unreachable();
}

extern "C" __attribute__((noreturn))
void UserStart() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG "Hello, world\r\n");

	LOADER::LoadContainer(info->RootContainer, (u8*)info->ManagerExecutableAddress, info->ManagerExecutableSize);

	HALT;

	__builtin_unreachable();
}
