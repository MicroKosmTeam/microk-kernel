#include <mm/heap.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <sys/mutex.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>

static void *heapStart;
static void *heapEnd;
static HeapSegHeader *lastHeader;
static bool isHeapActive = false;

static uint64_t freeMem;
static uint64_t totalMem;

static SpinLock HeapLock;

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

HeapSegHeader *HeapSegHeader::Split(size_t splitlength) {
        if (splitlength < 0x10) return NULL;
        int64_t splitSeglength = length - splitlength - (sizeof(HeapSegHeader));
        if (splitSeglength < 0x10) return NULL;

        HeapSegHeader *newSplitHeader = (HeapSegHeader*)((size_t)this + splitlength + sizeof(HeapSegHeader));
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

uint64_t GetFree() {
	return freeMem;
}

uint64_t GetTotal() {
	return totalMem;
}

void InitializeHeap(void *heapAddress, size_t pageCount) {
	KInfo *info = GetInfo();

        void *pos = heapAddress;
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Initializing the heap at 0x%x with %d pages.\r\n", heapAddress, pageCount);

	size_t pageListSize = sizeof(VMM::PageList) + pageCount * sizeof(VMM::PageMetadata);
	pageListSize += PAGE_SIZE - pageListSize % PAGE_SIZE;
	info->KernelHeapPageList = (VMM::PageList*)((uintptr_t)PMM::RequestPages(pageListSize / PAGE_SIZE) + info->HigherHalfMapping);
	info->KernelHeapPageList->PageCount = pageCount;
	info->KernelHeapPageList->AllocatedSize = pageListSize;

        for (size_t i = 0; i < pageCount; i++) {
		void *physical = PMM::RequestPage();
		info->KernelHeapPageList->Pages[i].IsCOW = false;
		info->KernelHeapPageList->Pages[i].Data.PhysicalAddress = (uintptr_t)physical;
		VMM::MapMemory(info->KernelVirtualSpace, physical, pos, VMM::VMM_PRESENT | VMM::VMM_READWRITE | VMM::VMM_GLOBAL | VMM::VMM_NOEXECUTE);
                pos = (void*)((size_t)pos + 0x1000); // Advancing
        }

        size_t heaplength = pageCount * 0x1000;

        heapStart = heapAddress;
        heapEnd = (void*)((size_t)heapStart + heaplength);

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

void *Malloc(size_t size) {
        if (size % 0x10 > 0){ // Not multiple of 0x10
                size -= (size % 0x10);
                size += 0x10;
        }

        if (size == 0) return NULL;

	for (int i = 0; i < MAX_TRIES; i++) {
		HeapLock.Lock();
		HeapSegHeader *currSeg = (HeapSegHeader*)heapStart;

		while(true) {
			if (currSeg-> free) {
				if (currSeg->length > size) {
					currSeg->Split(size);
					currSeg->free = false;
					freeMem -= size;
					HeapLock.Unlock();
					return (void*)((uint64_t)currSeg + sizeof(HeapSegHeader));
				} else if (currSeg->length == size) {
					currSeg->free = false;
					freeMem -= size;
					HeapLock.Unlock();
					return (void*)((uint64_t)currSeg + sizeof(HeapSegHeader));
				}
			}

			if (currSeg->next == NULL) break;
			currSeg = currSeg->next;
		}

		ExpandHeap(size * 4);

		HeapLock.Unlock();

	}

	return NULL;
}

void Free(void *address) {
	HeapLock.Lock();

        HeapSegHeader *segment = (HeapSegHeader*)address - 1;
	if(segment->free) { HeapLock.Unlock(); return; }
        segment->free = true;
        segment->CombineForward();
        segment->CombineBackward();
	freeMem += segment->length + sizeof(HeapSegHeader);

	HeapLock.Unlock();
}

void ExpandHeap(size_t length) {
	/* TODO: not yet implemented fully with KernelHeapPageList,
	 * we would need to map the new pages in each virtual space */
	PANIC("ExpandHeap not yet implemented fully");
	KInfo *info = GetInfo();

        if (length % 0x1000) { // We can't allocate less that a page
                length -= length % 0x1000;
                length += 0x1000;
        }

        size_t pageCount = length / 0x1000;
        HeapSegHeader *newSegment = (HeapSegHeader*)heapEnd;

	size_t newPageListSize = sizeof(VMM::PageList) + (info->KernelHeapPageList->PageCount + pageCount) * sizeof(uintptr_t);
	if(newPageListSize < info->KernelHeapPageList->AllocatedSize) {
		info->KernelHeapPageList->AllocatedSize = newPageListSize;

		size_t initialPageCount = info->KernelHeapPageList->PageCount;
		info->KernelHeapPageList->PageCount += pageCount;

		for (size_t i = initialPageCount; i < info->KernelHeapPageList->PageCount; i++) {
			void *physical = PMM::RequestPage();
			info->KernelHeapPageList->Pages[i].IsCOW = false;
			info->KernelHeapPageList->Pages[i].Data.PhysicalAddress = (uintptr_t)physical;
			VMM::MapMemory(info->KernelVirtualSpace, physical, heapEnd, VMM::VMM_PRESENT | VMM::VMM_READWRITE | VMM::VMM_GLOBAL | VMM::VMM_NOEXECUTE);
			heapEnd = (void*)((size_t)heapEnd + 0x1000);
		}

	} else {
		/* TODO: implement
		pageListSize += PAGE_SIZE - pageListSize % PAGE_SIZE;
		info->KernelHeapPageList = (VMM::PageList*)PMM::RequestPages(pageListSize / PAGE_SIZE);
		info->KernelHeapPageList->PageCount = pageCount;

		for (size_t i = ; i < info->KernelHeapPageList->PageCount; i++) {
			void *physical = PMM::RequestPage();
			info->KernelHeapPageList->PhysicalAddresses[i] = (uintptr_t)physical;
			VMM::MapMemory(info->KernelVirtualSpace, physical, heapEnd, VMM::VMM_PRESENT | VMM::VMM_READWRITE | VMM::VMM_GLOBAL | VMM::VMM_NOEXECUTE);
			heapEnd = (void*)((size_t)heapEnd + 0x1000);
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
