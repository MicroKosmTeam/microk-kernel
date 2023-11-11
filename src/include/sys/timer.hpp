#pragma once
#include <cstdint.hpp>
#include <dev/dev.hpp>

typedef imax stime;
typedef umax utime;

namespace TIME {
	void NDelay(DEV::Device *timer, usize nanoseconds);
	void UDelay(DEV::Device *timer, usize microseconds);
	void MDelay(DEV::Device *timer, usize milliseconds);
	void Delay(DEV::Device *timer, usize seconds);
}
