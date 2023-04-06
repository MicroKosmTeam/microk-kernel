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
	bool Locked;
};

class Spinlock : public Mutex {
public:
	void Lock() override {
		int *p = (int*)&Locked;
		while(!__sync_bool_compare_and_swap(p, 0, 1)) {
			while(*p) CPUPause();
		}
	}

	void Unlock() override {
		Locked = false;
	}

	bool IsLocked() override {
		return Locked;
	}
};
