#pragma once
#include <memory.hpp>
#include <capability.hpp>

namespace PMM {
	struct PhysicalMemoryManagerStruct {
		Capability *UntypedArea;

		bool IsActive;
	};

	void Init(Capability *untypedArea);
#define DEFAULT_CHECK_SPACE 5
	void CheckSpace(CapabilitySpace *cspace, usize count);
	void Deinit();

	void *RequestPage();
	void *RequestVirtualPage();
	void *RequestPages(usize length);
}
