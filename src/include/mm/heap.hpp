#pragma once
#include <cstdint.hpp>


struct HeapSegHeader{
        usize length;
        HeapSegHeader *next;
        HeapSegHeader *last;
        bool free;

        void CombineForward();
        void CombineBackward();
        HeapSegHeader *Split(usize splitLenght);
};

namespace HEAP {
	u64 GetFree();
	u64 GetTotal();

	bool IsHeapActive();

	void InitializeHeap(void *heapAddress, usize pageCount);
	void ExpandHeap(usize lenght);

	void *Malloc(usize size);
	void Free(void *address);
}
