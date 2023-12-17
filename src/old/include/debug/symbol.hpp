#pragma once
#include <cstdint.hpp>

namespace SYMBOL {
	struct SymbolPair {
		uptr Address;
		const char *Name;
	};

	uptr SymbolNameToAddress(const char *name);
	const char *AddressToSymbolName(uptr addr);
}
