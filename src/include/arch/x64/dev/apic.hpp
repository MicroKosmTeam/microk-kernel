#pragma once
#include <stdint.h>
#include <stddef.h>
#include <sys/time.hpp>
#include <dev/dev.hpp>

namespace x86_64 {
	struct APICTimer : public TIME::Timer {
	};

	struct APIC : public DEV::Device {
		uintptr_t Base;

		APICTimer *Timer;
	};

	int InitializeAPICTimer(TIME::Timer *timer);

	void EnableAPIC();
	uintptr_t GetAPICBase();
	void SetAPICBase(uintptr_t apic);
	void WriteAPICRegister(uint16_t offset, uint32_t data);
	void WaitAPIC();
	void SendAPICEOI();
	void SetAPICTimer(uint64_t newCycles);
	uint32_t ReadAPICRegister(uint16_t offset);
}
