#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

#include <sys/user.hpp>

typedef usize (*SyscallFunctionCallback)(usize,usize,usize,usize,usize,usize);

void InitSyscalls();

extern "C" usize HandleSyscall(usize syscallNumber, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5, usize arg6);

/* Kernel syscall handlers */
usize HandleSyscallDebugPrintK(const_userptr_t userString);
usize HandleSyscallMemoryVMAlloc(const_userptr_t userBase, usize length, usize flags);
usize HandleSyscallMemoryMMap(const_userptr_t userSrc, const_userptr_t userDest, usize length, usize flags);

usize HandleSyscallIPCQueue(userptr_t userCtlStruct);
usize HandleSyscallIPCMessageSend(usize queueID, const_userptr_t userMessagePointer, usize messageLength, usize messageType, usize messageFlags);
usize HandleSyscallIPCMessageReceive(usize queueID, userptr_t userMessageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);

usize HandleSyscallProcExit(usize exitCode);
