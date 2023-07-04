#pragma once
#include <stdint.h>
#include <stddef.h>

namespace DEV {
	struct BusNode {
		char BusName[256];

		uint32_t VendorID;
		uint32_t ProductID;
	
		BusNode *Next;
	};

	class BusManager {
	public:
		BusManager();

		int RegisterBusDriver(const char *busName, uint32_t vendorID, uint32_t productID);
		void GetBusDriver(const char *busName, uint32_t *vendorID, uint32_t *productID);
		void UnregisterBusDriver(const char *busName, uint32_t vendorID, uint32_t productID);
	private:
		BusNode *BaseNode;

		BusNode *AddNode(const char *busName, uint32_t vendorID, uint32_t productID);
		void RemoveNode(const char *busName, uint32_t vendorID, uint32_t productID);
		BusNode *FindNode(const char *busName, uint32_t vendorID, uint32_t productID, BusNode **previous, bool *found);

	};
}
