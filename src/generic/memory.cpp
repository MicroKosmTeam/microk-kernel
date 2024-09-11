#include <memory.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <vmm.hpp>
#include <kinfo.hpp>
#include <capability.hpp>
#include <math.hpp>

namespace MEM {
#ifdef UNDEF
void Init() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG "Physical Memory Map:\r\n");

	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		PRINTK::PrintK(PRINTK_DEBUG " [0x%x - 0x%x] -> %s\r\n",
				current->Base,
				current->Base + current->Length,
				MemoryTypeToString(current->Type));
	}

	/* Give correct type descriptors to the memory regions */
	PRINTK::PrintK(PRINTK_DEBUG 
			"Total kernel size: [0x%x - 0x%x] -> %d bytes\r\n"
			"        Essential: [0x%x - 0x%x] -> %d bytes\r\n"
			"             Text: [0x%x - 0x%x] -> %d bytes\r\n"
			"           ROData: [0x%x - 0x%x] -> %d bytes\r\n"
			"             Data: [0x%x - 0x%x] -> %d bytes\r\n"
			"          Dynamic: [0x%x - 0x%x] -> %d bytes\r\n"
			"              BSS: [0x%x - 0x%x] -> %d bytes\r\n",
			&__KernelBinaryEssentialStart, &__KernelBinaryBSSEnd, (&__KernelBinaryBSSEnd - &__KernelBinaryEssentialStart),
			&__KernelBinaryEssentialStart, &__KernelBinaryEssentialEnd, (&__KernelBinaryEssentialEnd - &__KernelBinaryEssentialStart),
			&__KernelBinaryTextStart, &__KernelBinaryTextEnd, (&__KernelBinaryTextEnd - &__KernelBinaryTextStart),
			&__KernelBinaryRODataStart, &__KernelBinaryRODataEnd, (&__KernelBinaryRODataEnd - &__KernelBinaryRODataStart),
			&__KernelBinaryDataStart, &__KernelBinaryDataEnd, (&__KernelBinaryDataEnd - &__KernelBinaryDataStart),
			&__KernelBinaryDynamicStart, &__KernelBinaryDynamicEnd, (&__KernelBinaryDynamicEnd - &__KernelBinaryDynamicStart),
			&__KernelBinaryBSSStart, &__KernelBinaryBSSEnd, (&__KernelBinaryBSSEnd - &__KernelBinaryBSSStart)
			);

	uptr essentialStartAddr = (uptr)&__KernelBinaryEssentialStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr essentialEndAddr = (uptr)&__KernelBinaryEssentialEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr textStartAddr = (uptr)&__KernelBinaryTextStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr textEndAddr = (uptr)&__KernelBinaryTextEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr rodataStartAddr = (uptr)&__KernelBinaryRODataStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr rodataEndAddr = (uptr)&__KernelBinaryRODataEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr dataStartAddr = (uptr)&__KernelBinaryDataStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr dataEndAddr = (uptr)&__KernelBinaryDataEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr dynamicStartAddr = (uptr)&__KernelBinaryDynamicStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr dynamicEndAddr = (uptr)&__KernelBinaryDynamicEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr bssStartAddr = (uptr)&__KernelBinaryBSSStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr bssEndAddr = (uptr)&__KernelBinaryBSSEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	ROUND_DOWN_TO_PAGE(essentialStartAddr);
	ROUND_DOWN_TO_PAGE(textStartAddr);
	ROUND_DOWN_TO_PAGE(rodataStartAddr);
	ROUND_DOWN_TO_PAGE(dataStartAddr);
	ROUND_DOWN_TO_PAGE(dynamicStartAddr);
	ROUND_DOWN_TO_PAGE(bssStartAddr);

	ROUND_UP_TO_PAGE(essentialEndAddr);
	ROUND_UP_TO_PAGE(textEndAddr);
	ROUND_UP_TO_PAGE(rodataEndAddr);
	ROUND_UP_TO_PAGE(dataEndAddr);
	ROUND_UP_TO_PAGE(dynamicEndAddr);
	ROUND_UP_TO_PAGE(bssEndAddr);

	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, essentialStartAddr, essentialEndAddr - essentialStartAddr, MEMMAP_KERNEL_ESSENTIALS);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, textStartAddr, textEndAddr - textStartAddr, MEMMAP_KERNEL_TEXT);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, rodataStartAddr, rodataEndAddr - rodataStartAddr, MEMMAP_KERNEL_RODATA);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, dataStartAddr, dataEndAddr - dataStartAddr, MEMMAP_KERNEL_DATA);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, dynamicStartAddr, dynamicEndAddr - dynamicStartAddr, MEMMAP_KERNEL_DYNAMIC);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, bssStartAddr, bssEndAddr - bssStartAddr, MEMMAP_KERNEL_BSS);

	/* Free bootloader-used memory that is no longer needed */
	/*
	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {

		if (current->Type == MEMMAP_BOOTLOADER_RECLAIMABLE) {
			PRINTK::PrintK(PRINTK_DEBUG " Freeing boot data: [0x%x - 0x%x]\r\n",
				       current->Base, current->Base + current->Length);

			current->Type = MEMMAP_USABLE;

			Memclr((void*)VMM::PhysicalToVirtual(current->Base), current->Length);
			PMM::FreePages((void*)current->Base, current->Length / PAGE_SIZE);
		}
	}*/
}
	
void Deinit() {
	KInfo *info = GetInfo();
	BOOTMEM::DeactivateBootMemory();

	/* Make sure we have enough preallocated space in the root capability space
	 * (+3 because if we want to split a region to create a new cnode we have the space
	 *  -> 2 for the split region UT, 1 for the node itself) */
	ROUND_UP_TO_PAGE(info->InitrdSize);
	usize memoryRegionsCount = MEMBLOCK::GetTotalElements(info->PhysicalMemoryChunks) + info->InitrdSize / PAGE_SIZE;
	usize cnodeSize = memoryRegionsCount * sizeof(Capability) + sizeof(CapabilityNode);
	ROUND_UP_TO_PAGE(cnodeSize);
	cnodeSize = MATH::UpperPowerOfTwoUSIZE(cnodeSize);
	usize cnodeSizeBits = MATH::GetPowerOfTwo(cnodeSize);

	/* Adding the memory map CNode to its fixed slot in the root CNode */
	CapabilityNode *rootNode = info->RootTCB->RootCNode;
	CapabilityNode *memoryNode;

	if (cnodeSize <= PAGE_SIZE) {
		memoryNode = CAPABILITY::CreateCNode(info->RootCSpace,
				        VMM::PhysicalToVirtual(
						(uptr)PMM::RequestPage()
					),
					cnodeSizeBits);
	} else {
		memoryNode = CAPABILITY::CreateCNode(info->RootCSpace,
				        VMM::PhysicalToVirtual(
						(uptr)PMM::RequestPages(cnodeSize)
					),
					cnodeSizeBits);
	}

	/* Creating the capability in the fixed slot */
	CAPABILITY::Originate(rootNode,
			      ROOT_CNODE_SLOTS::MEMORY_MAP_CNODE_SLOT,
			      (uptr)memoryNode,
			      OBJECT_TYPE::CNODE,
			      CAPABILITY_RIGHTS::ACCESS);

	/* Printing the final memory map by passing through all memblock regions */
	PRINTK::PrintK(PRINTK_DEBUG "Final Memory Map:\r\n");
	for (MEM::MEMBLOCK::MemblockRegion *current =
			(MEM::MEMBLOCK::MemblockRegion*)
			info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		PRINTK::PrintK(PRINTK_DEBUG " [0x%x - 0x%x] -> %s\r\n",
				current->Base,
				current->Base + current->Length,
				MemoryTypeToString(current->Type));
	}

	info->MemmapCapabilityEntries = 0;

	/* Cataloguing the memory all memblock regions and creating capabilities accordingly */
	for (MEM::MEMBLOCK::MemblockRegion *current =
			(MEM::MEMBLOCK::MemblockRegion*)
			info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		UntypedHeader *header = (UntypedHeader*)VMM::PhysicalToVirtual(current->Base);
		switch (current->Type) {
			case MEMMAP_USABLE:
				/* Usable is untyped, and userspace is free to do anything they please
				 * with it */
				header->Address = current->Base;
				header->Length = current->Length;
				header->Flags = 0;
				CAPABILITY::Originate(memoryNode,
						      (uptr)header,
						      OBJECT_TYPE::UNTYPED,
						      CAPABILITY_RIGHTS::ACCESS |
						      CAPABILITY_RIGHTS::RETYPE);

				info->MemmapCapabilityEntries++;
				break;
			case MEMMAP_KERNEL_DEVICE:
			case MEMMAP_FRAMEBUFFER:
			case MEMMAP_KERNEL_VMALLOC:
			case MEMMAP_KERNEL_AND_MODULES: {
				uptr initrdAddr = VMM::VirtualToPhysical(info->InitrdAddress);
				if (current->Base == initrdAddr) {
					for (usize i = 0; i < info->InitrdSize; i+=PAGE_SIZE) {
						CAPABILITY::Originate(memoryNode,
								      (uptr)(initrdAddr + i),
								      OBJECT_TYPE::FRAMES,
								      CAPABILITY_RIGHTS::ACCESS |
								      CAPABILITY_RIGHTS::READ);

						info->MemmapCapabilityEntries++;
					}

				}
				}
				break;
			default:
				break;
		}
	}

	((usize*)(VMM::PhysicalToVirtual(info->RootVirtualRegistersFrame)))[0] = info->MemmapCapabilityEntries;
	((usize*)(VMM::PhysicalToVirtual(info->RootVirtualRegistersFrame)))[1] = VMM::VirtualToPhysical(info->InitrdAddress);
	((usize*)(VMM::PhysicalToVirtual(info->RootVirtualRegistersFrame)))[2] = VMM::VirtualToPhysical(info->InitrdAddress) + info->InitrdSize;
}

const char *MemoryTypeToString(u8 type) {
		if (type >= 0x80) {
			type -= 0x80;

			const char *KernelMemTypeStrings[MEMMAP_KERNEL_SPECIFIC_COUNT] = {
				"Kernel Essentials",
				"Kernel Text",
				"Kernel ROData",
				"Kernel Data",
				"Kernel Dynamic",
				"Kernel BSS",
				"Kernel BootMem",
				"Kernel Bitmap",
				"Kernel Stack",
				"Kernel VMAlloc Memory",
				"Kernel Device Memory",
			};

			return KernelMemTypeStrings[type];
		}

		const char *GenericMemTypeStrings[MEMMAP_GENERIC_COUNT] = {
			"Usable",
			"Reserved",
			"ACPI Reclaimable",
			"ACPI NVS",
			"Bad",
			"Bootloader Reclaimable",
			"Kernel And Modules",
			"Framebuffer"
		};

		return GenericMemTypeStrings[type];
	}


#endif
}
