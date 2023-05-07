#pragma once
#include <sys/atomic.hpp>
#include <sys/printk.hpp>

class Mutex {
public:
	Mutex() {
		Locked = false;
	}

	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual bool IsLocked() = 0;
protected:
	bool Locked = false;
};

class SpinLock : public Mutex {
public:
	SpinLock() {
		Locked = false;
	}

	void Lock() override {
		bool *p = &Locked;
		while(!__sync_bool_compare_and_swap(p, false, true)) {
			while(*p) CPUPause();
		}
	}

	void Unlock() override {
		asm volatile ("" : : : "memory");
		Locked = false;
	}

	bool IsLocked() override {
		return Locked;
	}
};

bool TryLockMutex(bool *lock);
void SpinlockLockMutex(bool *lock);

inline void LockMutex(bool *lock) {
	SpinlockLockMutex(lock);
}

void UnlockMutex(bool *lock);
