#pragma once
#include <stdint.h>

class Timer {
public:
	virtual void InitTimer() = 0;
	virtual void Wait(uint64_t microseconds, uint16_t method) = 0;
protected:
};


#include <arch/x64/dev/hpet.hpp>
class TSCTimer : public Timer{
public:
	TSCTimer() : TSCCyclesPerMicroseconds(0) {
	}

	void InitTimer() override {
		CalibrateTSCWith(0, &TSCCyclesPerMicroseconds);
	}

	void Wait(uint64_t microseconds, uint16_t method) override {
		uint64_t tscEnd = __builtin_ia32_rdtsc() + microseconds * TSCCyclesPerMicroseconds;

		while (__builtin_ia32_rdtsc() < tscEnd) {
		}
	}

private:
	uint64_t TSCCyclesPerMicroseconds;
};
