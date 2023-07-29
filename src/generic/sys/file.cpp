#include <sys/file.hpp>
#include <init/kinfo.hpp>
#include <mm/string.hpp>
#include <sys/printk.hpp>

struct RSDP2 {
	unsigned char Signature[8];
	uint8_t Checksum;
	uint8_t OEMID[6];
	uint8_t Revision;
	uint32_t RSDTAddress;
	uint32_t Length;
	uint64_t XSDTAddress;
	uint8_t ExtendedChecksum;
	uint8_t Reserved[3];
}__attribute__((packed));

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
		char tableName[5];
		memcpy(tableName, name, 4);
		tableName[4] = '\0';

		if (strcmp(tableName, "RSDP") == 0) {
			*size = ((RSDP2*)info->RSDP)->Length;
			return info->RSDP;
		} else {
			*size = 0;
			return NULL;
		}
	} else if (strcmp(id, "FB") == 0) {
		size_t num = atoi(name);

		if (num > info->framebufferCount) {
			*size = 0;
			return NULL;
		}

		Framebuffer *fbData = &info->framebuffers[num];

		memcpy(fbData->Address, fbData, sizeof(Framebuffer));

		*size = fbData->Width * fbData->Height * fbData->BPP;
		return fbData->Address;

	}
	
	*size = 0;
	return 0;
}
}
