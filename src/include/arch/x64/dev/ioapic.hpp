#pragma once
#include <stdint.h>
#include <stddef.h>

namespace x86_64 {
	void EnableIOAPIC(uintptr_t address);
	void WriteIOAPICRegister(uintptr_t base, uint16_t offset, uint64_t data);
	uint64_t ReadIOAPICRegister(uintptr_t base, uint16_t offset);
}
