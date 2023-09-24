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
#include <init/kargs.hpp>
#include <init/kinfo.hpp>

void ParseArgs() {
	/* Get our arguments string from the info->KernelArgs variable
	   and see if they are available */
	KInfo *info = GetInfo();
	if(info->KernelArgs == NULL) return;

	/* We split the values in the id and value pairs */
	const char *id = Strtok((char*)info->KernelArgs, "=");
	if(id == NULL) return;
	const char *val = Strtok(NULL, " ");
	if (val == NULL) return ;

	while(true) {
		/* Here we parse the IDs.
		  For now, we just Strcmp what we desire,
		  in the future a hashmap would be probably better */
		if(Strcmp(id, "user") == 0) {
			/* Select the user file */
			Strcpy(info->UserModuleName, "/");
			Strcpy(info->UserModuleName + 1, val);
		} else {
			/* Invalid argument */
		}

		/* Continue with the parsing,
		   checking we have not reached the end of the string every time*/
		id = Strtok(NULL, "=");
		if(id == NULL) break;
		val = Strtok(NULL, " ");
		if (val == NULL) break;
	}

}
