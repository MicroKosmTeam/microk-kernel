#include <sys/file.hpp>
#include <init/kinfo.hpp>
#include <mm/string.hpp>

uintptr_t FindFile(const char *path, size_t *size) {
	KInfo *info = GetInfo();

	for (size_t i = 0; i < info->FileCount; i++) {
		if (Strncmp(info->BootFiles[i].Path, path) == 0) {
			*size = info->BootFiles[i].Size;
			return info->BootFiles[i].Address;
		}
	}

	*size = 0;
	return 0;
}
