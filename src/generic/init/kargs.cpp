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
 * File: generic/init/kargs.cpp
 */

#include <mm/string.hpp>
#include <sys/printk.hpp>
#include <init/kargs.hpp>
#include <init/kinfo.hpp>
#include <dev/earlycon.hpp>

void ParseArgs() {
	/* Get our arguments string from the info->KernelArgs variable
	   and see if they are available */
	KInfo *info = GetInfo();
	if (info->KernelArgs == NULL) return;
	
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "MicroKosm is booted. Cmdline: %s\r\n", info->KernelArgs);

	/* We split the values in the id and value pairs */
	char *endChar;
	char *id = Strtok((char*)info->KernelArgs, "=", &endChar);
	if (id == NULL) return;
	char *val = Strtok(NULL, " ", &endChar);
	if (val == NULL) return ;

	while (true) {
		/* Here we parse the IDs.
		  For now, we just Strncmp what we desire,
		  in the future a hashmap would be probably better */
		if (Strncmp(id, "init", MAX_CMDLINE_ARGUMENT_LENGTH) == 0) {
			/* Select the user file */
			Strncpy(info->UserModuleName, val, MAX_FILE_NAME_LENGTH);
		} else if (Strncmp(id, "loglevel", MAX_CMDLINE_ARGUMENT_LENGTH) == 0) {
		} else if (Strncmp(id, "earlycon", MAX_CMDLINE_ARGUMENT_LENGTH) == 0) {
			info->KernelPort = (DEV::UART::UARTDevice*)DEV::EARLYCON::InitializeEarlycon(val);
		} else if (Strncmp(id, "", MAX_CMDLINE_ARGUMENT_LENGTH) == 0) {
		} else {
			/* Invalid argument */
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Unknown argument: %s\r\n", id);
		}

		/* Continue with the parsing,
		   checking we have not reached the end of the string every time*/
		id = Strtok(NULL, "=", &endChar);
		if(id == NULL) break;
		val = Strtok(NULL, " ", &endChar);
		if (val == NULL) break;
	}

}
