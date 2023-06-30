#pragma once
#include <mkmi_module.h>
#include <proc/process.hpp>

namespace MODULE {
namespace Manager {
	void Init();
	uint64_t RegisterModule(MKMI_Module module, PROC::Process *mainProcess);
	uint64_t UnregisterModule(MKMI_ModuleID id);
	MKMI_Module *GetModule(MKMI_ModuleID id);
}
}
