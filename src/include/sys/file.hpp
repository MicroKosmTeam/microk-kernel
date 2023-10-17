#pragma once
#include <cdefs.h>
#include <cstdint.hpp>


struct BootFile {
	uptr Address;
	usize Size;
	char Path[256];
	char Cmdline[256];
}__attribute__((packed));

uptr FindFile(const char *path, usize *size);
