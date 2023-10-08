#pragma once
#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>

struct BootFile {
	uintptr_t Address;
	size_t Size;
	char Path[256];
	char Cmdline[256];
}__attribute__((packed));

uintptr_t FindFile(const char *path, size_t *size);
