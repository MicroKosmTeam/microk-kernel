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
isize HandleSyscallMemoryPMAlloc(userptr_t baseDestination, usize length, usize flags);
isize HandleSyscallMemoryVMFree(const_userptr_t userBase, usize length);
isize HandleSyscallMemoryMMap(uptr src, uptr dest, usize length, usize flags);
isize HandleSyscallMemoryMProtect(uptr base, usize length, usize flags);
isize HandleSyscallMemoryUnMap(uptr base, usize length);
isize HandleSyscallMemoryInOut(const_userptr_t iorequests, usize count);
isize HandleSyscallMemoryRequestResources(const_userptr_t requests, usize count);


isize HandleSyscallProcFork();
isize HandleSyscallProcExec(userptr_t executableBase, usize executableSize);
isize HandleSyscallProcReturn(usize returnCode);
isize HandleSyscallProcExit(usize exitCode);
isize HandleSyscallProcWait(usize TODO);
isize HandleSyscallProcSendSig(usize TODO);

isize HandleSyscallIPCMessageSend(usize queue, const_userptr_t messagePointer, usize messageLength, usize messageFlags);
isize HandleSyscallIPCMessageReceive(usize queue, userptr_t messageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags);
isize HandleSyscallIPCMessageQueue(usize queue, usize operation);

