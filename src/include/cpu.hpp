#pragma once
#include <object.hpp>

namespace ARCH {
	void InitializeBootCPU();
	void InitializeCPUFeatures();

	__attribute__((noreturn))
	void GoToUserspace(SchedulerContext *context);

}
