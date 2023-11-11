#pragma once
#include <cstdint.hpp>

namespace SYMBOL {
	uptr SymbolNameToAddress(const char *name);
	const char *AddressToSymbolName(uptr addr);
}
