#pragma once
#include <stdint.h>
#include <stddef.h>

#define HPET_COUNTER_LOW_OFFSET   0x00
#define HPET_COUNTER_HIGH_OFFSET  0x04
#define HPET_PERIOD_OFFSET        0x10
#define HPET_CONFIG_OFFSET        0x20

namespace x86_64 {
	void WriteHPETRegister(uintptr_t baseAddress, uint32_t offset, uint32_t value);
}
