#pragma once
#include <object.hpp>

namespace TASK {
	ThreadControlBlock *CreateThread(VirtualSpace *space, SchedulerContext *context);
	SchedulerContext *CreateSchedulerContext();
}
