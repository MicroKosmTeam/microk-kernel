#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

#include <sys/user.hpp>

typedef isize (*SyscallFunctionCallback)(usize,usize,usize,usize,usize,usize);

void InitSyscalls();

extern "C" isize HandleSyscall(usize syscallNumber, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5, usize arg6);

/* Kernel syscall handlers */
isize HandleSyscallDebugPrintK(const_userptr_t userString);
isize HandleSyscallMemoryVMAlloc(const_userptr_t userBase, usize length, usize flags);
isize HandleSyscallProcExit(usize exitCode);
