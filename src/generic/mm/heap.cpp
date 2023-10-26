#include <mm/heap.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>

static void *heapStart;
static void *heapEnd;
static HeapSegHeader *lastHeader;
static bool isHeapActive = false;

static u64 freeMem;
static u64 totalMem;

void HeapSegHeader::CombineForward() {
        if(next == NULL) return;
        if(!next->free) return;

        if(next == lastHeader) lastHeader = this;

        if(next->next != NULL) {
                next->next->last = this;
        }

        length = length + next->length + sizeof(HeapSegHeader);

        next = next->next;
}

void HeapSegHeader::CombineBackward() {
        if (last != NULL && last->free) last->CombineForward();
}

HeapSegHeader *HeapSegHeader::Split(usize splitlength) {
        if (splitlength < 0x10) return NULL;
        i64 splitSeglength = length - splitlength - (sizeof(HeapSegHeader));
        if (splitSeglength < 0x10) return NULL;

        HeapSegHeader *newSplitHeader = (HeapSegHeader*)((usize)this + splitlength + sizeof(HeapSegHeader));
	if(next == NULL) next = newSplitHeader;
	else next->last = newSplitHeader;            // Set the next segment's last segment to our new segment
        newSplitHeader->next = next;            // Set the new segment's next segment to our original next
        next = newSplitHeader;                  // Set our new segment to the next segment
        newSplitHeader->last = this;            // Set our new segment's last segment to us
        newSplitHeader->length = splitSeglength;// Set the new header's length
        newSplitHeader->free = free;            // Make sure both free are the same
        length = splitlength;                   // Set the original segment's length to the nes one

        if (lastHeader == this) lastHeader = newSplitHeader;
        return newSplitHeader;
}

namespace HEAP {
bool IsHeapActive() {
	return isHeapActive;
}

u64 GetFree() {
	return freeMem;
}

u64 GetTotal() {
	return totalMem;
}

void InitializeHeap(void *heapAddress, usize pageCount) {
	KInfo *info = GetInfo();

        uptr pos = (uptr)heapAddress;
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Initializing the heap at 0x%x with %d pages.\r\n", heapAddress, pageCount);

	usize pageListSize = sizeof(VMM::PageList) + pageCount * sizeof(VMM::PageMetadata);
	pageListSize += PAGE_SIZE - pageListSize % PAGE_SIZE;
	info->KernelHeapPageList = (VMM::PageList*)((uptr)PMM::RequestPages(pageListSize / PAGE_SIZE) + info->HigherHalfMapping);
	info->KernelHeapPageList->PageCount = pageCount;
	info->KernelHeapPageList->AllocatedSize = pageListSize;

        for (usize i = 0; i < pageCount; i++) {
		uptr physical = (uptr)PMM::RequestPage();
		info->KernelHeapPageList->Pages[i].IsCOW = false;
		info->KernelHeapPageList->Pages[i].Data.PhysicalAddress = physical;
		VMM::MapPage(info->KernelVirtualSpace, physical, pos, VMM_FLAGS_KERNEL_DATA); 
                pos = pos + PAGE_SIZE; // Advancing
        }

        usize heaplength = pageCount * PAGE_SIZE;

        heapStart = heapAddress;
        heapEnd = (void*)((usize)heapStart + heaplength);

        HeapSegHeader *startSeg = (HeapSegHeader*)heapAddress;
        startSeg->length = heaplength - sizeof(HeapSegHeader);
        startSeg->next = NULL;
        startSeg->last = NULL;
        startSeg->free = true;
        lastHeader = startSeg;

	totalMem = freeMem = heaplength;

	isHeapActive = true;
}

#define MAX_TRIES 4

void *Malloc(usize size) {
        if (size % 0x10 > 0){ // Not multiple of 0x10
                size -= (size % 0x10);
                size += 0x10;
        }

        if (size == 0) return NULL;

	for (int i = 0; i < MAX_TRIES; i++) {
		HeapSegHeader *currSeg = (HeapSegHeader*)heapStart;

		while(true) {
			if (currSeg-> free) {
				if (currSeg->length > size) {
					currSeg->Split(size);
					currSeg->free = false;
					freeMem -= size;
					return (void*)((u64)currSeg + sizeof(HeapSegHeader));
				} else if (currSeg->length == size) {
					currSeg->free = false;
					freeMem -= size;
					return (void*)((u64)currSeg + sizeof(HeapSegHeader));
				}
			}

			if (currSeg->next == NULL) break;
			currSeg = currSeg->next;
		}

		ExpandHeap(size * 4);
	}

	return NULL;
}

void Free(void *address) {
        HeapSegHeader *segment = (HeapSegHeader*)address - 1;
	if(segment->free) { return; }
        segment->free = true;
        segment->CombineForward();
        segment->CombineBackward();
	freeMem += segment->length + sizeof(HeapSegHeader);
}

void ExpandHeap(usize length) {
	/* TODO: not yet implemented fully with KernelHeapPageList,
	 * we would need to map the new pages in each virtual space */
	PANIC("ExpandHeap not yet implemented fully");
	KInfo *info = GetInfo();

        if (length % 0x1000) { // We can't allocate less that a page
                length -= length % 0x1000;
                length += 0x1000;
        }

        usize pageCount = length / 0x1000;
        HeapSegHeader *newSegment = (HeapSegHeader*)heapEnd;

	usize newPageListSize = sizeof(VMM::PageList) + (info->KernelHeapPageList->PageCount + pageCount) * sizeof(uptr);
	if(newPageListSize < info->KernelHeapPageList->AllocatedSize) {
		info->KernelHeapPageList->AllocatedSize = newPageListSize;

		usize initialPageCount = info->KernelHeapPageList->PageCount;
		info->KernelHeapPageList->PageCount += pageCount;

		for (usize i = initialPageCount; i < info->KernelHeapPageList->PageCount; i++) {
			uptr physical = (uptr)PMM::RequestPage();
			info->KernelHeapPageList->Pages[i].IsCOW = false;
			info->KernelHeapPageList->Pages[i].Data.PhysicalAddress = (uptr)physical;
			VMM::MapPage(info->KernelVirtualSpace, physical, (uptr)heapEnd, VMM_FLAGS_KERNEL_DATA); 
			heapEnd = (void*)((uptr)heapEnd + 0x1000);
		}

	} else {
		/* TODO: implement
		pageListSize += PAGE_SIZE - pageListSize % PAGE_SIZE;
		info->KernelHeapPageList = (VMM::PageList*)PMM::RequestPages(pageListSize / PAGE_SIZE);
		info->KernelHeapPageList->PageCount = pageCount;

		for (usize i = ; i < info->KernelHeapPageList->PageCount; i++) {
			void *physical = PMM::RequestPage();
			info->KernelHeapPageList->PhysicalAddresses[i] = (uptr)physical;
			VMM::MapMemory(info->KernelVirtualSpace, physical, heapEnd, VMM::VMM_PRESENT | VMM::VMM_READWRITE | VMM::VMM_GLOBAL | VMM::VMM_NOEXECUTE);
			heapEnd = (void*)((usize)heapEnd + 0x1000);
		} */

	}

        newSegment->free = true;
        newSegment->last = lastHeader;
        lastHeader->next = newSegment;
        lastHeader = newSegment;
        newSegment->next = NULL;
        newSegment->length = length - sizeof(HeapSegHeader);
        newSegment->CombineBackward();

	totalMem += length;
	freeMem += length;

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "%d bytes memory allocated.\r\n", length);
}
}
