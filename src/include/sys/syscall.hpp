#pragma once
#include <stddef.h>

extern "C" void HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);
