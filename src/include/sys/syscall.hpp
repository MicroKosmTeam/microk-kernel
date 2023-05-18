#pragma once
#include <stddef.h>

#define SYSCALL_DEBUG_PRINTK   0x000001
#define SYSCALL_MEMORY_VMALLOC 0x000002

size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);
