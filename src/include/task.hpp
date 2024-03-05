#pragma once
#include <object.hpp>

namespace TASK {
	ThreadControlBlock *InitializeTCB(uptr frame);
	SchedulerContext *InitializeContext(uptr frame, uptr ip, uptr sp);
}
