#include <syscall.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <panic.hpp>
#include <cdefs.h>

inline __attribute__((always_inline))
usize GetVirtualArgs(ThreadControlBlock *tcb, usize index) {
	if (index * sizeof(usize) >= VIRTUAL_REGISTERS_SIZE) {
		return 0;
	}

	return ((usize*)tcb->VirtualRegisters)[index];
}

#ifdef UNDEF
inline __attribute__((always_inline))
void SyscallCapCtl(ThreadControlBlock *tcb, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	CapabilityNode *nodePtr = tcb->RootCNode;

	if (nodePtr == NULL) {
		/* Task isn't allowed to use capabilities */
		OOPS("Not allowed");
		return;
	}

	Capability *cspaceCap = &nodePtr->Slots[CSPACE_SLOT];
	CapabilitySpace *cspace = (CapabilitySpace*)cspaceCap->Object;

	usize operation = firstArgument;
	Capability *nodeCap = (Capability*)secondArgument;
	usize nodeSlot = thirdArgument;

	if (nodeCap == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "Using root node\r\n");
	} else {
		nodePtr = CAPABILITY::ResolveCNode(nodeCap);
		if (CAPABILITY::IsNodeInSpace(cspace, nodePtr) != 0) {
			OOPS("Node isn't in space");
			return;
		}
	}

	if (CAPABILITY::IsCapabilityInNode(nodePtr, nodeSlot)) {
		OOPS("Invalid CAP addressing");
		return;
	}

	switch (operation) {
		case SYSCALL_CAPCTL_GET_PTR: {
			uptr *newPtr = (uptr*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];

			if (capability->Type == OBJECT_TYPE::RESERVED_SLOT) {
				OOPS("Addressing reserved CAP slot");
				*newPtr = 0;
				return;
			}

			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-accessible CAP slot");
				*newPtr = 0;
				return;
			}

			*newPtr = (uptr)capability;
			}
			break;
		case SYSCALL_CAPCTL_GET_NEXT_PTR: {
			uptr *newPtr = (uptr*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];

			if (capability->Type == OBJECT_TYPE::RESERVED_SLOT) {
				OOPS("Addressing reserved CAP slot");
				*newPtr = 0;
				return;
			}

			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-accessible CAP slot");
				*newPtr = 0;
				return;
			}

			*newPtr = (uptr)capability->Next;
			}
			break;
		case SYSCALL_CAPCTL_GET_NEXT_SLOT: {
			usize *newSlot = (uptr*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];

			if (capability->Type == OBJECT_TYPE::RESERVED_SLOT) {
				OOPS("Addressing reserved CAP slot");
				*newSlot = 0;
				return;
			}

			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-accessible CAP slot");
				*newSlot = 0;
				return;
			}

			if (capability->Next == NULL) {
				*newSlot = -1;
			} else {
				*newSlot = CAPABILITY::GetCapabilitySlot(nodePtr, capability->Next);
			}
			}
			break;
		case SYSCALL_CAPCTL_GET_OBJ: {
			usize *newObjPtr = (usize*)fourthArgument;
			u8 *newKindPtr = (u8*)fithArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];
		
			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-addressable CAP slot");
				*newObjPtr = -1;
				*newKindPtr = -1;
				return;
			}

			if (capability->IsMasked) {
				*newObjPtr = -1;
				*newKindPtr = capability->Type;

				return;
			}

			*newObjPtr = capability->Object;
			*newKindPtr = capability->Type;
			}
			break;
		case SYSCALL_CAPCTL_GET_UT: {
			UntypedHeader *newPtr = (UntypedHeader*)fourthArgument;
			Capability *capability = &nodePtr->Slots[nodeSlot];
		
			if (capability->Type != OBJECT_TYPE::UNTYPED) {
				*(usize*)newPtr = -1;
				return;
			}


			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-addressable CAP slot");
				*(usize*)newPtr = -1;
				return;
			}

			UntypedHeader *header = (UntypedHeader*)VMM::PhysicalToVirtual(capability->Object);
			*newPtr = *header;
			}
			break;
		case SYSCALL_CAPCTL_ADD_CNODE: {
			Capability *capability = &nodePtr->Slots[nodeSlot];
		
			CapabilityNode *newNodePtr = CAPABILITY::ResolveCNode(capability);

			if ((capability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}

			CAPABILITY::AddCNode(cspace, newNodePtr); 

			}
			break;
		case SYSCALL_CAPCTL_SPLIT: {
			usize splitSize = fourthArgument;
			Capability *newNodeCap = (Capability*)fithArgument;
			usize *newSlot = (usize*)sixthArgument;
			usize count = GetVirtualArgs(tcb, 7);

			CapabilityNode *newNodePtr = CAPABILITY::ResolveCNode(newNodeCap);
			if (CAPABILITY::IsNodeInSpace(cspace, newNodePtr) != 0) {
				OOPS("Node isn't in space");
				return;
			}

			Capability *ut = &nodePtr->Slots[nodeSlot];
	
			if (ut->Type != OBJECT_TYPE::UNTYPED) {
				OOPS("Splitting non-ut cap");
				*newSlot = -1;
				return;
			}
	
			if ((ut->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}
			
			PRINTK::PrintK(PRINTK_DEBUG "Respective node ptrs: 0x%x vs 0x%x\r\n", nodePtr, newNodePtr);

			if (newNodePtr != nodePtr) {
				/* We must do a derive before */
				ut = CAPABILITY::Derive(nodePtr, ut, newNodePtr);

				if (ut == NULL) {
					OOPS("Failed to derive in split");
					return;
				}
			}

			Capability *capability = CAPABILITY::Split(newNodePtr, ut, splitSize, count);
			*newSlot = CAPABILITY::GetCapabilitySlot(newNodePtr, capability);
			}
			break;
		case SYSCALL_CAPCTL_RETYPE: {
			OBJECT_TYPE type = (OBJECT_TYPE)fourthArgument;
			Capability *newNodeCap = (Capability*)fithArgument;
			usize *newSlot = (usize*)sixthArgument;
			u16 accessRights = GetVirtualArgs(tcb, 7);
			// TODO;
			/* FIX THIS ENORMOUS SECURITY VULNERABILITY */

			CapabilityNode *newNodePtr = CAPABILITY::ResolveCNode(newNodeCap);
			if (CAPABILITY::IsNodeInSpace(cspace, newNodePtr) != 0) {
				OOPS("Node isn't in space");
				return;
			}

			Capability *ut = &nodePtr->Slots[nodeSlot];

			if (ut->Type != OBJECT_TYPE::UNTYPED) {
				OOPS("Retyping non-ut cap");
				return;
			}

			if ((ut->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}

			PRINTK::PrintK(PRINTK_DEBUG "Respective node ptrs: 0x%x vs 0x%x\r\n", nodePtr, newNodePtr);
			if (newNodePtr != nodePtr) {
				/* We must do a derive before */
				ut = CAPABILITY::Derive(nodePtr, ut, newNodePtr);

				if (ut == NULL) {
					OOPS("Failed to derive in retype");
					return;
				}
			}

			Capability *capability = CAPABILITY::Retype(newNodePtr, ut, type, accessRights);
			*newSlot = CAPABILITY::GetCapabilitySlot(newNodePtr, capability);
			}
			break;
		case SYSCALL_CAPCTL_MAP_INTERMEDIATE: {
			usize level = fourthArgument;
			uptr virt = fithArgument;
			usize flags = VMM::ConvertUserFlags(sixthArgument);

			//usize upperSlot = GetVirtualArgs(tcb, 7);

			Capability *pageStructure = &nodePtr->Slots[nodeSlot];
			if (pageStructure->Type != OBJECT_TYPE::PAGING_STRUCTURE) {
				OOPS("Mapping non PAGE cap");
				return;
			}

			if ((pageStructure->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}
/*
			Capability *upperCapability = &nodePtr->Slots[upperSlot];
			if (upperCapability->Type != OBJECT_TYPE::PAGING_STRUCTURE) {
				return;
			}

			if ((upperCapability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}*/
			
			PRINTK::PrintK(PRINTK_DEBUG "Mapping intermediate from 0x%x to 0x%x with flags 0x%x\r\n", pageStructure->Object, virt, flags);
			usize result = VMM::MapIntermediateLevel(tcb->MemorySpace, level, pageStructure->Object, virt, flags);
			if (result == 0 || result == 1) {
/*				pageStructure->Parent = upperCapability;
				++upperCapability->Children;*/
			} else {
				OOPS("Mapping intermediate slot failed");
			}
			}
			break;
		case SYSCALL_CAPCTL_MAP_PAGE: {
			uptr virt = fourthArgument;
			usize flags = VMM::ConvertUserFlags(fithArgument);
			//usize upperSlot = (usize)sixthArgument;

			Capability *frame = &nodePtr->Slots[nodeSlot];
			if (frame->Type != OBJECT_TYPE::FRAMES) {
				OOPS("Mapping non PAGE cap");
				return;
			}

			if ((frame->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}

			/*
			Capability *upperCapability = &nodePtr->Slots[upperSlot];
			if (upperCapability->Type != OBJECT_TYPE::PAGING_STRUCTURE) {
				return;
			}

			if ((upperCapability->AccessRights & CAPABILITY_RIGHTS::ACCESS) == 0) {
				OOPS("Addressing non-seeable CAP slot");
				return;
			}*/
			PRINTK::PrintK(PRINTK_DEBUG "Mapping from 0x%x to 0x%x with flags 0x%x\r\n", frame->Object, virt, flags);
			usize result = VMM::MapPage(tcb->MemorySpace, frame->Object, virt, flags);
			if (result == 0) {
				/* Perfect */
			} else {
				OOPS("Mapping page slot failed");
			}
			}
			break;
		case SYSCALL_CAPCTL_REPLYRECV:{
			Capability *tcbNodeCap = (Capability*)fourthArgument;
			Capability *tcbCap = (Capability*)fithArgument;
			
			Capability *ep = &nodePtr->Slots[nodeSlot];

			(void)tcbNodeCap, (void)tcbCap, (void) ep;

			}
			break;
		case SYSCALL_CAPCTL_CALL:{
			}
			break;
		case SYSCALL_CAPCTL_DEBUG: {
			CAPABILITY::DumpCNode(nodePtr);
			}
			break;
		default:
			break;
	}
}
#endif
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

		case SYSCALL_VECTOR_CAPCTL:
			//SyscallCapCtl(tcb, firstArgument, secondArgument, thirdArgument, fourthArgument, fithArgument, sixthArgument);
			break;
		case SYSCALL_VECTOR_ARCHCTL:
			break;
		case SYSCALL_VECTOR_YEILD:
			break;
		default:
			break;
	}
}
