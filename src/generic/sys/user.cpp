#include <sys/user.hpp>
#include <cdefs.h>
#include <mm/memory.hpp>
#include <mm/string.hpp>

#define LOWER_HALF_CEILING  0x00007FFFFFFFF000
#define USER_MEMORY_CEILING 0x0000700000000000

int CheckUserMemory(const_userptr_t userAddress, usize length) {
	uptr address = (uptr)userAddress;

	if (address > USER_MEMORY_CEILING ||
	    length > USER_MEMORY_CEILING ||
	    address + length > USER_MEMORY_CEILING)
		return -EINVALID;

	return 0;
}

void CopyFromUser(void *kernelAddress, const_userptr_t userAddress, usize length) {
	Memcpy(kernelAddress, (void*)userAddress, length);
}

void CopyStringFromUser(char *kernelAddress, const_userptr_t userAddress, usize maxLength) {
	Strncpy(kernelAddress, (const char*)userAddress, maxLength);
}

void CopyToUser(const void *kernelAddress, userptr_t userAddress, usize length) {
	Memcpy((void*)userAddress, (void*)kernelAddress, length);
}

void CopyStringToUser(const char *kernelAddress, userptr_t userAddress, usize maxLength) {
	Strncpy((char*)userAddress, kernelAddress, maxLength);
}
