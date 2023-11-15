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
isize HandleSyscallMemoryMMap(const_userptr_t userSrc, const_userptr_t userDest, usize length, usize flags);

isize HandleSyscallIPCQueue(userptr_t userCtlStruct);
isize HandleSyscallIPCMessageSend(usize queueID, const_userptr_t userMessagePointer, usize messageLength, usize messageType, usize messageFlags);
isize HandleSyscallIPCMessageReceive(usize queueID, userptr_t userMessageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);

isize HandleSyscallProcExit(usize exitCode);
