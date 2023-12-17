#pragma once
#include <cstdint.hpp>



namespace DEV::CPU {
	struct TopologyStructure {
		u32 ID;
		
		TopologyStructure *Parent;

		TopologyStructure *Next;
		TopologyStructure *Previous;

		struct TopologyList {
			TopologyStructure *First;
			TopologyStructure *Last;
		} Children;

		void *ArchitectureSpecificInformation;
	};

	TopologyStructure *CreateTopologyStructure(TopologyStructure *parent, u32 id);
	TopologyStructure *FindTopologyStructure(TopologyStructure *parent, u32 id);

	int InitializeBootCPU();

	usize GetContextSize();
	void UpdateCPUContext(u8 *context, uptr sp, uptr ip, usize argc, usize argv, bool user);
}
