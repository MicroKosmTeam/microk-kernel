#pragma once


typedef usize (*SyscallFunctionCallback)(usize,usize,usize,usize,usize,usize);

void InitSyscalls();

extern "C" usize HandleSyscall(usize syscallNumber, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5, usize arg6);
