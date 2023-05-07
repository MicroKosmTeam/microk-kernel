#pragma once
#include <mkmi.h>

namespace MODULE {
namespace Manager {
	void Init();
	uint64_t RegisterModule(MKMI_Module module);
	uint64_t UnregisterModule(MKMI_ModuleID id);
	MKMI_Module *GetModule(MKMI_ModuleID id);
}
}
