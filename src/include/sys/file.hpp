#pragma once
#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>

namespace FILE {
#ifdef CONFIG_KERNEL_MODULES
	struct BootFile {
		void *Address;
		uint64_t Size;
		char *Path;
		char *Cmdline;
	};
#endif


	void *Open(char *path, size_t *size);
}
