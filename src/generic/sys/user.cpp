#include <sys/user.hpp>
#include <cdefs.h>
#include <mm/memory.hpp>

int CheckUserMemory(const_userptr_t userAddress, size_t length) {
	(void)userAddress;
	(void)length;

	return -EINVALID;
}

void CopyFromUser(void *kernelAddress, const_userptr_t userAddress, size_t length) {
	Memcpy(kernelAddress, (void*)userAddress, length);
}

void CopyToUser(const void *kernelAddress, userptr_t userAddress, size_t length) {
	Memcpy((void*)userAddress, (void*)kernelAddress, length);
}

