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
 * File: include/init/main.hpp
 */

#pragma once

/*
 * function: EarlyKernelInit
 * This function is supposed to be called by bootloader interfaces
 * to allocated the KInfo struct, initialize basic architecture-specific,
 * devices such as the serial port used for early printk.
 * 
 * arguments: void 
 * return: int
 * Serves to tell if the initialization was successful. If a the value is
 * non-zero, abort execution.
 */
int EarlyKernelInit();

/*
 * function: PrintBanner 
 * This function just prints some basic kernel information for
 * debugging purposes. It serves to show the kernel has successfully
 * initialized.
 * 
 * arguments: void 
 * return: void 
 */
void PrintBanner();

/*
 * function: PrintBanner 
 * This function just prints some basic kernel information for
 * debugging purposes. It serves to show the kernel has successfully
 * initialized.
 * 
 * arguments: void 
 * return: never 
 */
__attribute__((noreturn)) void KernelStart();
