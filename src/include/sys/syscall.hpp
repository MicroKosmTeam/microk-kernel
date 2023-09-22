#pragma once
#include <stddef.h>

typedef size_t (*SyscallFunctionCallback)(size_t,size_t,size_t,size_t,size_t,size_t);

void InitSyscalls();

extern "C" size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6);

void AddOverride(size_t syscallNumber);
size_t CheckOverride(size_t syscallNumber);
size_t RunOverride(size_t syscallNumber);
