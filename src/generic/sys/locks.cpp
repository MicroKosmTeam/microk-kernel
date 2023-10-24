#include <sys/locks.hpp>

extern "C" void CPUPause();

bool SpinlockTryLock(bool *lock) {
	if(__sync_bool_compare_and_swap(lock, false, true)) {
		return true;
	}

	return false;
}

void SpinlockLock(bool *lock) {
	while(!__sync_bool_compare_and_swap(lock, false, true)) {
		while (*lock) {
			CPUPause();
		}
	}
}

void SpinlockUnlock(bool *lock) {
	asm volatile ("":::"memory");
	*lock = false;
}
