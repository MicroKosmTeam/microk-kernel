#include <sys/timer.hpp>

namespace TIME {
void NDelay(DEV::Device *timer, usize nanoseconds) {
	(void)timer;
	(void)nanoseconds;
}

void UDelay(DEV::Device *timer, usize microseconds) {
	(void)timer;
	(void)microseconds;
}

void MDelay(DEV::Device *timer, usize milliseconds) {
	(void)timer;
	(void)milliseconds;
}

void Delay(DEV::Device *timer, usize seconds) {
	(void)timer;
	(void)seconds;
}

}
