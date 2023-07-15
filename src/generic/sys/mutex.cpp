#include <sys/mutex.hpp>

bool TryLockMutex(bool *lock) {
	if(__sync_bool_compare_and_swap(lock, false, true)) {
		return true;
	}

	return false;
}

void SpinlockLockMutex(bool *lock) {
	while(!__sync_bool_compare_and_swap(lock, false, true));
}

void UnlockMutex(bool *lock) {
	*lock = false;
}
