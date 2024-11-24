#include <syscall.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <panic.hpp>
#include <cdefs.h>

extern "C" void SyscallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	KInfo *info = GetInfo();
	CapabilitySpace *cspace = &info->RootContainer->CSpace;
	VirtualSpace vspace = info->RootContainer->MemorySpace;
	(void)vspace;

	VMM::LoadVirtualSpace(vspace);

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
		case SYSCALL_VECTOR_ADDRESS_CAPABILITY: {
			Capability *cap = CAPABILITY::AddressFirstCapability(cspace, firstArgument, (OBJECT_TYPE)secondArgument);
			*(uptr*)thirdArgument = cap->Object;
			}
			break;
		case SYSCALL_VECTOR_SEARCH_CAPABILITY:
			*(uptr*)thirdArgument = (uptr)CAPABILITY::AddressFirstCapability(cspace, firstArgument, (OBJECT_TYPE)secondArgument);
			break;
		case SYSCALL_VECTOR_RETYPE_CAPABILITY: {
			Capability *cap = CAPABILITY::AddressFirstCapability(cspace, firstArgument, UNTYPED_FRAMES);
			OBJECT_TYPE type = (OBJECT_TYPE)secondArgument;
			usize retypeCount = fourthArgument;
			Capability **frameRetypeArray = (Capability**)thirdArgument;
			CAPABILITY::RetypeUntyped(cspace, cap, type, retypeCount, frameRetypeArray);
			}
			break;
		case SYSCALL_VECTOR_UNTYPE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_SPLIT_CAPABILITY: {
			usize splitCount = fourthArgument;
			usize splitSize = thirdArgument;
			Capability **splitArray = (Capability**)secondArgument;

			Capability *cap = CAPABILITY::AddressFirstCapability(cspace, firstArgument, UNTYPED_FRAMES);
			if (cap == NULL) {
				splitArray[0] = NULL;
			}

			if(CAPABILITY::SplitUntyped(cspace, cap, splitSize, splitCount, splitArray) == NULL) {
				splitArray[0] = NULL;
			}
			}
			break;
		case SYSCALL_VECTOR_MERGE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_GET_FREE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_ADD_FREE_CAPABILITY:
			break;
		case SYSCALL_VECTOR_MAP_CAPABILITY: {
			OBJECT_TYPE type = (OBJECT_TYPE)secondArgument;
			if (type < FRAME_MEMORY || type >= CACHE_MEMORY) {
				break;
			}

			Capability *cap = CAPABILITY::AddressFirstCapability(cspace, firstArgument, type);
			if(cap != NULL) {
				VMM::MapPage(vspace, thirdArgument, VMM::VirtualToPhysical(cap->Object), fourthArgument);
			}
			}
			break;
		case SYSCALL_VECTOR_MAP_INTERMEDIATE_CAPABILITY: {
			Capability *cap = CAPABILITY::AddressFirstCapability(cspace, firstArgument, VIRTUAL_MEMORY_PAGING_STRUCTURE);
			if (cap == NULL) {
				break;
			}
				
			VMM::MapIntermediateLevel(vspace, secondArgument, VMM::VirtualToPhysical(cap->Object), thirdArgument, fourthArgument);
			}
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
	
	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
}
