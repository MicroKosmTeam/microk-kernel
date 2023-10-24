#pragma once
#include <sys/atomic.hpp>
#include <sys/printk.hpp>

bool SpinlockTryLock(bool *lock);

void SpinlockLock(bool *lock);
void SpinlockUnlock(bool *lock);
