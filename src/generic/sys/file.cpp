#include <sys/file.hpp>
#include <init/kinfo.hpp>
#include <mm/string.hpp>

uptr FindFile(const char *path, usize *size) {
	KInfo *info = GetInfo();

	for (usize i = 0; i < info->FileCount; i++) {
		if (Strncmp(info->BootFiles[i].Path, path, MAX_FILE_NAME_LENGTH) == 0) {
			*size = info->BootFiles[i].Size;
			return info->BootFiles[i].Address;
		}
	}

	*size = 0;
	return 0;
}
