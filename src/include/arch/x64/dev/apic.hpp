#pragma once
#include <cstdint.hpp>

#include <sys/time.hpp>
#include <dev/dev.hpp>

namespace x86_64 {
	struct APICTimer : public TIME::Timer {
	};

	struct APIC : public DEV::Device {
		uptr Base;

		APICTimer *Timer;
	};

	int InitializeAPICTimer(TIME::Timer *timer);

	void EnableAPIC();
	uptr GetAPICBase();
	void SetAPICBase(uptr apic);
	void WriteAPICRegister(u16 offset, u32 data);
	void WaitAPIC();
	void SendAPICEOI();
	void SetAPICTimer(u64 newCycles);
	u32 ReadAPICRegister(u16 offset);
}
