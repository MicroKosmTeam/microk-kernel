#include <syscall.hpp>
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	(void)firstArgument;
	(void)secondArgument;
	(void)thirdArgument;
	(void)fourthArgument;
	(void)fithArgument;
	(void)sixthArgument;

	switch (syscallNumber) {
		case SYSCALL_VECTOR_DEBUG:
			break;
		case SYSCALL_VECTOR_YEILD:
			break;
		case SYSCALL_VECTOR_CALL:
			break;
		case SYSCALL_VECTOR_REPLYRECV:
			break;
		default:
			break;
	}
}
