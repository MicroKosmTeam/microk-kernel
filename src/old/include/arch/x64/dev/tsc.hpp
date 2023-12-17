#pragma once
#include <cstdint.hpp>
#include <dev/dev.hpp>

namespace x86_64::TSC {
	struct TSC : public DEV::Device {
		usize NanoTickRate;
		usize MicroTickRate;
		usize MilliTickRate;
		usize TickRate;
	};

	inline __attribute__((always_inline))
	usize RDTSC() {
		return __builtin_ia32_rdtsc();
	}

	DEV::Device *CreateTSCDevice();

	int InitializeDevice(DEV::Device *device, va_list ap);
	int DeinitializeDevice(DEV::Device *device, va_list ap);
	intmax_t Ioctl(DEV::Device *device, request_t request, va_list ap);

	void NDelay(TSC *tsc, usize nanoseconds);
	void UDelay(TSC *tsc, usize microseconds);
	void MDelay(TSC *tsc, usize milliseconds);
	void Delay(TSC *tsc, usize milliseconds);
}
