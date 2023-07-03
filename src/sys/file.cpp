#include <sys/file.hpp>
#include <init/kinfo.hpp>
#include <mm/string.hpp>
#include <dev/acpi/acpi.hpp>
#include <sys/printk.hpp>

namespace FILE {
void *Open(char *path, size_t *size) {
	KInfo *info = GetInfo();

	const char *id = strtok(path, ":");
	const char *name = strtok(NULL, ":");

	if (strcmp(id, "FILE") == 0) {
		for (int i = 0; i < info->fileCount; i++) {
			if (strcmp(info->bootFiles[i].Path, name) == 0) {
				*size = info->bootFiles[i].Size;
				return info->bootFiles[i].Address - info->higherHalfMapping;
			}
		}
	} else if (strcmp(id, "ACPI") == 0) {
		if (strcmp(name, "RSDP") == 0) {
			*size = ((ACPI::RSDP2*)info->RSDP)->Length;
			return info->RSDP;
		} else {
			void *addr = ACPI::GetXSDT();
			if (addr == NULL) {
				*size = 0;
				return NULL;
			}

			addr = ACPI::FindTable(addr, name);
			*size = (addr == NULL) ? 0 : ((ACPI::SDTHeader*)addr)->Length;
			return addr;
		}
	} else if (strcmp(id, "FB") == 0) {
		size_t num = atoi(name);

		if (num > info->framebufferCount) {
			*size = 0;
			return NULL;
		}

		Framebuffer *fbData = &info->framebuffers[num];

		memcpy(fbData->Address, fbData, sizeof(Framebuffer));
		PRINTK::PrintK("Data copied.\r\n");

		*size = fbData->Width * fbData->Height * fbData->BPP;
		PRINTK::PrintK("Size assigned.\r\n");
		return fbData->Address;

	}
	
	*size = 0;
	return 0;
}
}
