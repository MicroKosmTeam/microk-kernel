#include <sys/timer.hpp>
#include <arch/x64/dev/apic.hpp>

namespace TIMER {
void WaitForCycles(size_t cycles) {
	x86_64::WaitAPIC(cycles);
}

}
