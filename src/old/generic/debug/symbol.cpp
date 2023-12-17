#include <debug/symbol.hpp>
#include <init/kinfo.hpp>

namespace SYMBOL {
uptr SymbolNameToAddress(const char *name) {
	(void)name;
	return 0;
}

const char *AddressToSymbolName(uptr addr) {
	KInfo *info = GetInfo();

	uptr lowestHigherAddr = -1;
	const char *name = "Unknown";

	if (info->KernelSymbolTable == NULL) {
		return name;
	}

	SymbolPair *pair = &info->KernelSymbolTable[0];
	
	while(pair->Address != ~((uptr)0) && pair->Name != NULL) {
		if (pair->Address >= addr && pair->Address < lowestHigherAddr) {
			lowestHigherAddr = pair->Address;
			name = pair->Name; 
		}

		++pair;
	}

	return name;
}
}
