#include <memory.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <pmm.hpp>
#include <vmm.hpp>
#include <bootmem.hpp>
#include <kinfo.hpp>
#include <capability.hpp>
#include <math.hpp>

namespace MEM {
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


	/* Initializing virtual memory */
	VMM::InitVMM();

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

	/* Make sure we have enough preallocated space in the root capability space */
	usize memoryRegionsCount = MEMBLOCK::GetTotalElements(info->PhysicalMemoryChunks);
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
						      CAPABILITY_RIGHTS::SEE |
						      CAPABILITY_RIGHTS::RETYPE);
				break;
			case MEMMAP_KERNEL_DEVICE:
			case MEMMAP_FRAMEBUFFER:
			case MEMMAP_KERNEL_VMALLOC:
			default:
				break;
		}
	}
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


}

extern "C" int memcmp(const void* buf1, const void* buf2, usize count) {
	return Memcmp(buf1, buf2, count);
}

extern "C" void *memcpy(void *dest, void *src, usize n) {
	return Memcpy(dest, src, n);
}

extern "C" void *memmove(void *dest, const void *src, usize n) {
	return Memmove(dest, src, n);
}

extern "C" void *memset(void *start, u8 value, usize num) {
	return Memset(start, value, num);
}

void *Memclr(void *start, usize num) {
	return Memset(start, 0, num);
}

int Memcmp(const void* buf1, const void* buf2, usize count) {
	if(!count)
		return(0);

	while(--count && *(char*)buf1 == *(char*)buf2 ) {
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}

	return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

void *Memcpy(void *dest, void *src, usize n) {
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	for (usize i=0; i<n; i++) cdest[i] = csrc[i];

	return dest;
}

void *Memmove(void *dest, const void *src, usize n) {
	u8 *pdest = (u8 *)dest;
	const u8 *psrc = (const u8 *)src;

	if (src > dest) {
		for (usize i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (usize i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}

void *Memset(void *start, u8 value, usize num) {
	num /= sizeof(u8);

	for (usize i = 0; i < num; i++) {
		*(u8*)((uptr)start + i) = value;
	}


	return start;
}
