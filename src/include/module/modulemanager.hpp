#pragma once
#include <stdint.h>
#include <proc/process.hpp>
#include <module/module.hpp>

namespace MODULE {
	struct ModuleNode {
		Module *ModuleData;

		ModuleNode *Next;
	};

	class Manager {
	public:
		Manager();

		int RegisterModule(PROC::ProcessBase *process, uint32_t vendorID, uint32_t productID);
		void UnregisterModule(uint32_t vendorID, uint32_t productID);
		Module *GetModule(uint32_t vendorID, uint32_t productID);
		Module *GetModule(size_t PID);
	private:
		ModuleNode *BaseNode;

		ModuleNode *AddNode(uint32_t vendorID, uint32_t productID);
		void RemoveNode(uint32_t vendorID, uint32_t productID);
		ModuleNode *FindNode(uint32_t vendorID, uint32_t productID, ModuleNode **previous, bool *found);
	};
}
