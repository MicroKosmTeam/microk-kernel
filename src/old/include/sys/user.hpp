#pragma once
#include <cstdint.hpp>

#define USER_READ_MEMORY_CEILING  0x0000800000000000
#define USER_ALLOC_MEMORY_CEILING 0x0000700000000000

/* Simple __userptr struct to make sure userptr_t and
 * const_userptr_t don't get mixed up with other pointers
 */
struct __userptr { char _dummy; };
typedef __userptr *userptr_t;
typedef const __userptr *const_userptr_t;

int CheckUserMemory(const_userptr_t userAddress, usize length);

void CopyFromUser(void *kernelAddress, const_userptr_t userAddress, usize length);
void CopyStringFromUser(char *kernelAddress, const_userptr_t userAddress, usize maxLength);

void CopyToUser(const void *kernelAddress, userptr_t userAddress, usize length);
void CopyStringToUser(const char *kernelAddress, userptr_t userAddress, usize maxLength);

extern "C" void EnterUserspace(void *function, void *stack);
extern "C" void ExitUserspace();
