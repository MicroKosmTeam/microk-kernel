#include <hypercall.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <panic.hpp>
#include <cdefs.h>

extern "C" void HypercallMain(usize syscallNumber, usize firstArgument, usize secondArgument, usize thirdArgument, usize fourthArgument, usize fithArgument, usize sixthArgument) {
	KInfo *info = GetInfo();
	// TODO
	CapabilitySpace *cspace = &info->RootContainer->CSpace;
	VirtualSpace vspace = info->RootContainer->MemorySpace;

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
			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);
			//*(uptr*)fourthArgument = (uptr)cap;
			*(Capability*)secondArgument = *cap;
			}
			break;
		case SYSCALL_VECTOR_GET_UNTYPED_CAPABILITY: {
			Capability *copyCap = (Capability*)firstArgument;
			usize index = secondArgument;
			Capability *cap = CAPABILITY::GetUntyped(cspace, index);
			if (cap != NULL) {
				*copyCap = *cap;
			} else {
				Capability cap;
				cap.IsMasked = 1;
				cap.Type = NULL_CAPABILITY;
				cap.Object = -1;
				cap.Size = 0;
				*copyCap = cap;
			}
			}
			break;
		case SYSCALL_VECTOR_RETYPE_CAPABILITY: {
			OBJECT_TYPE type = (OBJECT_TYPE)secondArgument;
			usize retypeCount = fourthArgument;
			Capability *copyArray = (Capability*)thirdArgument;
			Capability *frameRetypeArray[retypeCount];

			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);
			

			CAPABILITY::RetypeUntyped(cspace, cap, type, retypeCount, frameRetypeArray);

			for (usize i = 0; i < retypeCount; ++i) {
				copyArray[i] = *frameRetypeArray[i];
			}
			}
			break;
		case SYSCALL_VECTOR_UNTYPE_CAPABILITY:
			// TODO
			break;
		case SYSCALL_VECTOR_SPLIT_CAPABILITY: {
			usize splitCount = fithArgument;
			usize splitSize = thirdArgument;
			usize splitOffset = fourthArgument;
			Capability *copyArray = (Capability*)secondArgument;
			Capability *splitArray[splitCount];

			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);
			if (cap == NULL) {
				splitArray[0] = NULL;
			}

			if(CAPABILITY::SplitUntyped(cspace, cap, splitSize, splitOffset, splitCount, splitArray) == NULL) {
				splitArray[0] = NULL;
			}

			for (usize i = 0; i < splitCount; ++i) {
				copyArray[i] = *splitArray[i];
			}
			}
			break;
		case SYSCALL_VECTOR_MERGE_CAPABILITY:
			// TODO
			break;
		case SYSCALL_VECTOR_GET_FREE_CAPABILITY:
			// TODO
			break;
		case SYSCALL_VECTOR_ADD_FREE_CAPABILITY: {
			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);

			CAPABILITY::AddSlabNode(cspace, cap);
			}

			break;
		case SYSCALL_VECTOR_MAP_CAPABILITY: {
			OBJECT_TYPE type = (OBJECT_TYPE)secondArgument;
			if (type < FRAME_MEMORY || type >= CACHE_MEMORY) {
				PRINTK::PrintK(PRINTK_DEBUG "Invalid mapping type");
				break;
			}

			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);
			if(cap != NULL) {
				//PRINTK::PrintK(PRINTK_DEBUG "Mapping page 0x%x at addr 0x%x\r\n", cap->Object, thirdArgument);
				VMM::MapPage(vspace, cap->Object, thirdArgument, VMM::ConvertUserFlags(fourthArgument));
			}
			}
			break;
		case SYSCALL_VECTOR_MAP_INTERMEDIATE_CAPABILITY: {
			Capability *cap = CAPABILITY::AddressCapability(cspace, firstArgument);
			if (cap == NULL) {
				break;
			}
				
			//PRINTK::PrintK(PRINTK_DEBUG "Mapping level paging\r\n");
			VMM::MapIntermediateLevel(vspace, secondArgument, cap->Object, thirdArgument, VMM::ConvertUserFlags(fourthArgument));
			}
			break;
			// TODO
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
