#include <syscall.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <panic.hpp>
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	switch (syscallNumber) {
		case SYSCALL_VECTOR_DEBUG: {
			usize value = firstArgument;
			usize type = secondArgument;
			switch(type) {
				case 's':
					PRINTK::PrintK(PRINTK_DEBUG "%s", (const char*)value);
					break;
				case 'c': {
					char str[2];
					str[0] = value;
					str[1] = '\0';
					PRINTK::PrintK(PRINTK_DEBUG "%s", (const char*)str);
					}
					break;
				case 'd':
					PRINTK::PrintK(PRINTK_DEBUG "%d", (long)value);
					break;
				case 'x':
					PRINTK::PrintK(PRINTK_DEBUG "%x", value);
					break;
				default:
					break;
			}
			}
			break;
		case SYSCALL_VECTOR_ADDRESS_CAPABILITY:
			
			break;
		case SYSCALL_VECTOR_SEARCH_CAPABILITY:
			break;
		case SYSCALL_VECTOR_SEARCH_NEAR_CAPABILITY:
			break;
		case SYSCALL_VECTOR_RETYPE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_UNTYPE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_SPLIT_CAPABILITY:
			break;
		case SYSCALL_VECTOR_MERGE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_GET_FREE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_ADD_FREE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_START_CONTAINER:
			break;
		case SYSCALL_VECTOR_STOP_CONTAINER:
			break;
		case SYSCALL_VECTOR_CREATE_CONTAINER:
			break;
		case SYSCALL_VECTOR_CLONE_CONTAINER:
			break;
		case SYSCALL_VECTOR_DESTROY_CONTAINER:
			break;
		default:
			break;
	}
}
