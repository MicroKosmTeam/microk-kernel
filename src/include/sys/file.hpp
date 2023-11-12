#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

struct BootFile {
	uptr Address;
	usize Size;
	char Path[MAX_FILE_NAME_LENGTH];
	char Cmdline[MAX_FILE_NAME_LENGTH];
}__attribute__((packed));

uptr FindFile(const char *path, usize *size);
uptr FindFileCmdline(const char *path, usize *size, const char **cmdline);
