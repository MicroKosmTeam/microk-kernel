#include <syscall.hpp>
#include <printk.hpp>
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	switch (syscallNumber) {
		case SYSCALL_VECTOR_DEBUG: {
			const char *string = (const char*)firstArgument;

			PRINTK::PrintK(PRINTK_DEBUG "%s", string);
			}
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
	
	firstArgument = secondArgument = 
	thirdArgument = fourthArgument = 
	fithArgument = sixthArgument = 0;

}
