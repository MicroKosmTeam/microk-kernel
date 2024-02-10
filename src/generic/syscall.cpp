#include <syscall.hpp>
#include <printk.hpp>
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	(void)firstArgument;
	(void)secondArgument;
	(void)thirdArgument;
	(void)fourthArgument;
	(void)fithArgument;
	(void)sixthArgument;

	PRINTK::PrintK(PRINTK_DEBUG "Hello, syscall!\r\n");

	switch (syscallNumber) {
		case SYSCALL_VECTOR_DEBUG:
			PRINTK::PrintK(PRINTK_DEBUG " -> Debug\r\n");
			break;
		case SYSCALL_VECTOR_YEILD:
			PRINTK::PrintK(PRINTK_DEBUG " -> Yeild\r\n");
			break;
		case SYSCALL_VECTOR_CALL:
			PRINTK::PrintK(PRINTK_DEBUG " -> Call\r\n");
			break;
		case SYSCALL_VECTOR_REPLYRECV:
			PRINTK::PrintK(PRINTK_DEBUG " -> ReplyRecv\r\n");
			break;
		default:
			break;
	}
}
