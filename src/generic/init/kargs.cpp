#include <init/kargs.hpp>
#include <init/kinfo.hpp>
#include <mm/string.hpp>

void ParseArgs() {
	KInfo *info = GetInfo();

	if(info->KernelArgs == NULL) return;

	const char *id = strtok(info->KernelArgs, "=");
	if(id == NULL) return;
	const char *val = strtok(NULL, " ");
	if (val == NULL) return ;

	while(true) {
		if(strcmp(id, "user") == 0) {
			strcpy(info->UserModuleName, "FILE:/");
			strcpy(info->UserModuleName + 6, val);
		} else {
			/* Parse arch args? */
		}

		id = strtok(NULL, "=");
		if(id == NULL) break;
		val = strtok(NULL, " ");
		if (val == NULL) break;
	}

}
