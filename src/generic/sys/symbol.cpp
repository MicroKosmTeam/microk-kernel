#include <sys/symbol.hpp>
#include <mm/string.hpp>

const Symbol *LookupSymbol(const char *name) {
	uint64_t symbolIndex;

	for (symbolIndex = 0; symbolIndex < symbolCount; symbolIndex++) {
		if (Strncmp(symbols[symbolIndex].name, name) == 0) return &symbols[symbolIndex - 1];
	}

	return NULL;

}

const Symbol *LookupSymbol(uint64_t addr) {
	uint64_t symbolIndex;

	if (addr < symbols[0].addr || addr > symbols[symbolCount - 1].addr) return NULL;

	for (symbolIndex = 0; symbolIndex < symbolCount; symbolIndex++) {
		if (symbols[symbolIndex].addr > addr) break;
	}

	return &symbols[symbolIndex - 1];
}
