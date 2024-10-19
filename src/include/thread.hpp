#pragma once
#include <object.hpp>

namespace TASK {
	ThreadControlBlock *InitializeTCB(uptr frame);

	SchedulerContext *InitializeContext(uptr frame, uptr ip, uptr sp, uptr argc, uptr argv);
	__attribute__((always_inline)) inline
	SchedulerContext *InitializeContext(uptr frame, uptr ip, uptr sp) {
		return InitializeContext(frame, ip, sp, 0, 0);
	}
}
