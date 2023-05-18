#include <sys/syscall.hpp>
#include <stdarg.h>
#include <stdint.h>

#include <sys/printk.hpp>
size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
	switch(syscallNumber) {
		case SYSCALL_DEBUG_PRINTK: {
			char *string = arg1;

			PRINTK::PrintK("%s", string);
			}
			break;
		default:
			break;
	}

	return arg1;
}
