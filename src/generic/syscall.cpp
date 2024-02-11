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
				case 'x':
					PRINTK::PrintK(PRINTK_DEBUG "0x%x", value);
					break;
				default:
					break;
			}
			}
			break;
		case SYSCALL_VECTOR_YEILD:
			PRINTK::PrintK(PRINTK_DEBUG " -> Yeild\r\n");
			break;
		case SYSCALL_VECTOR_CAPCTL: {
			KInfo *info = GetInfo();
			PRINTK::PrintK(PRINTK_DEBUG " -> CapCtl\r\n");

			CapabilityNode *nodePtr = (CapabilityNode*)firstArgument;
			usize nodeSlot = secondArgument;
			uptr *newPtr = (uptr*)thirdArgument;
			usize *newSize = (usize*)fourthArgument;

			if (nodePtr == NULL) {
				nodePtr = CAPABILITY::GetRootNode(info->RootCapabilitySpace);
			}

			if (nodeSlot >= info->CapabilityNodeSize / sizeof(Capability)) {
				PANIC("Invalid CAP addressing");
			}

			Capability *cap = &nodePtr->Slots[nodeSlot];
			*newPtr = cap->Object;
			*newSize = cap->Size;
			}
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
