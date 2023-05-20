#include <sys/syscall.hpp>
#include <stdarg.h>
#include <stdint.h>
#include <cdefs.h>

#include <sys/printk.hpp>

void HandleSyscallDebugPrintK(char *string);

void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack);
void HandleSyscallProcExit(size_t exitCode, uintptr_t stack);

void HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
	switch(syscallNumber) {
		case SYSCALL_DEBUG_PRINTK:
			HandleSyscallDebugPrintK(arg1);
			break;
		case SYSCALL_PROC_RETURN:
			HandleSyscallProcReturn(arg1, arg2);
			break;
		case SYSCALL_PROC_EXIT:
			HandleSyscallProcExit(arg1, arg2);
			break;
		default:
			break;
	}
}

void HandleSyscallDebugPrintK(char *string) {
	PRINTK::PrintK("%s", string);
}

void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 
}

void HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
}
