#pragma once
#include <stddef.h>

/* Simple __userptr struct to make sure userptr_t and
 * const_userptr_t don't get mixed up with other pointers
 */
struct __userptr { char _dummy; };
typedef __userptr *userptr_t;
typedef const __userptr *const_userptr_t;

int CheckUserMemory(const_userptr_t userAddress, size_t length);
void CopyFromUser(void *kernelAddress, const_userptr_t userAddress, size_t length);
void CopyToUser(const void *kernelAddress, userptr_t userAddress, size_t length);

extern "C" void EnterUserspace(void *function, void *stack);
extern "C" void ExitUserspace();
