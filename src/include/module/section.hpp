#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MODULE {
	struct SectionNode {
		char SectionName[256];

		uint32_t VendorID;
		uint32_t ProductID;
	
		SectionNode *Next;
	};

	class SectionManager {
	public:
		SectionManager();

		int RegisterSectionDriver(const char *sectionName, uint32_t vendorID, uint32_t productID);
		void GetSectionDriver(const char *sectionName, uint32_t *vendorID, uint32_t *productID);
		void UnregisterSectionDriver(const char *sectionName, uint32_t vendorID, uint32_t productID);
	private:
		SectionNode *BaseNode;

		SectionNode *AddNode(const char *sectionName, uint32_t vendorID, uint32_t productID);
		void RemoveNode(const char *sectionName, uint32_t vendorID, uint32_t productID);
		SectionNode *FindNode(const char *sectionName, uint32_t vendorID, uint32_t productID, SectionNode **previous, bool *found);

	};
}
