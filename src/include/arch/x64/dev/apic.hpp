#pragma once
#include <stdint.h>
#include <stddef.h>

namespace x86_64 {
	void EnableAPIC();
	uintptr_t GetAPICBase();
	void SetAPICBase(uintptr_t apic);
	void WriteAPICRegister(uint16_t offset, uint32_t data);
	void WaitAPIC(uint32_t cycles);
	void SendAPICEOI();
	void StartAPICTimer();
	uint32_t ReadAPICRegister(uint16_t offset);
}
