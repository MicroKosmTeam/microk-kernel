#include <sys/symbol.hpp>
#include <mm/string.hpp>

namespace SYMBOL {
uptr SymbolNameToAddress(const char *name) {
	(void)name;
	return 0;
}

const char *AddressToSymbolName(uptr addr) {
	(void)addr;
	return "Unknown";
}
}
