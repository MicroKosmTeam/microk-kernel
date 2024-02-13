#include <syscall.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <panic.hpp>
#include <cdefs.h>

#ifdef UNDEF
inline __attribute__((always_inline))
void SyscallCapCtl(usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	KInfo *info = GetInfo();
	PRINTK::PrintK(PRINTK_DEBUG " -> CapCtl\r\n");

	CapabilitySpace *cspace = info->RootCapabilitySpace;

	usize operation = firstArgument;
	Capability *nodeCap = (Capability*)secondArgument;
	CapabilityNode *nodePtr;
	usize nodeSlot = thirdArgument;

	if (nodeCap == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "Using root node\r\n");
		nodePtr = CAPABILITY::GetRootNode(cspace);
	} else {
		/* If the address of nodeCap is invalid, a page fault will occur, killing the running process */
		nodePtr = (CapabilityNode*)nodeCap->Object;

		if (CAPABILITY::IsNodeInSpace(cspace, nodePtr) != 0) {
			OOPS("Node isn't is space");
			return;
		}
	}

	if (nodeSlot >= info->CapabilityNodeSize / sizeof(Capability)) {
		OOPS("Invalid CAP addressing");
		return;
	}

	switch (operation) {
		case SYSCALL_CAPCTL_GET_PTR: {
			uptr *newPtr = (uptr*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];

			if (capability->Type == ObjectType::RESERVED_SLOT) {
				OOPS("Addressing reserved CAP slot");
				*newPtr = 0;
				return;
			}

			if ((capability->AccessRights & CapabilityRights::ACCESS) == 0) {
				OOPS("Addressing non-accessible CAP slot");
				*newPtr = 0;
				return;
			}

			*newPtr = (uptr)capability;
			}
			break;
		case SYSCALL_CAPCTL_GET_CAP: {
			Capability *newPtr = (Capability*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];
		
			if (capability->Type == ObjectType::RESERVED_SLOT) {
				OOPS("Addressing reserved CAP slot");
				*(usize*)newPtr = 0;
				return;
			}

			if ((capability->AccessRights & CapabilityRights::SEE) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				*(usize*)newPtr = 0;
				return;
			}

			newPtr->Type = capability->Type;
			newPtr->Object = capability->Object;
			newPtr->Size = capability->Size;
			newPtr->AccessRights = capability->AccessRights;
			}
			break;
		case SYSCALL_CAPCTL_RETYPE: {
			}
			break;
		default:
			break;
	}

	(void)fithArgument;
	(void)sixthArgument;
}

inline __attribute__((always_inline))
void SyscallArchCtl(usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	KInfo *info = GetInfo();
	PRINTK::PrintK(PRINTK_DEBUG " -> ArchCtl\r\n");

	usize operation = firstArgument;
	int *result = (int*)secondArgument;
	
	/* MAPPING SIMPLIFIED FOR TESTING PURPOSES */
	switch (operation) {
		case SYSCALL_ARCHCTL_MAP_INTERMEDIATE: {
			usize level = thirdArgument;
			uptr frame = fourthArgument;
			uptr virt = fithArgument;
			usize flags = VMM::ConvertUserFlags(sixthArgument);
			*result = VMM::MapIntermediateLevel(info->RootTCB->MemorySpace, level, frame, virt, flags);
			}
			break;
		case SYSCALL_ARCHCTL_MAP_PAGE: {
			uptr phys = thirdArgument;
			uptr virt = fourthArgument;
			usize flags = VMM::ConvertUserFlags(fithArgument);
			*result = VMM::MapPage(info->RootTCB->MemorySpace, phys, virt, flags);
			}
			break;
		default:
			break;
	}

}
#endif

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	(void)firstArgument;
	(void)secondArgument;
	(void)thirdArgument;
	(void)fourthArgument;
	(void)fithArgument;
	(void)sixthArgument;
	switch (syscallNumber) {
		case SYSCALL_VECTOR_DEBUG: {
			usize value = firstArgument;
			usize type = secondArgument;
			switch(type) {
				case 's':
					PRINTK::PrintK(PRINTK_DEBUG "%s", (const char*)value);
					break;
				case 'd':
					PRINTK::PrintK(PRINTK_DEBUG "%d", (long)value);
					break;
				case 'x':
					PRINTK::PrintK(PRINTK_DEBUG "0x%x", value);
					break;
				default:
					break;
			}
			}
			break;
/*
		case SYSCALL_VECTOR_CAPCTL:
			SyscallCapCtl(firstArgument, secondArgument, thirdArgument, fourthArgument, fithArgument, sixthArgument);
			break;
		case SYSCALL_VECTOR_ARCHCTL:
			SyscallArchCtl(firstArgument, secondArgument, thirdArgument, fourthArgument, fithArgument, sixthArgument);
			break;
		case SYSCALL_VECTOR_TASKCTL:
			SyscallArchCtl(firstArgument, secondArgument, thirdArgument, fourthArgument, fithArgument, sixthArgument);
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
*/
		default:
			break;
	}
}
