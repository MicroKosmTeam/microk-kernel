#include <sys/symbol.hpp>
#include <mm/string.hpp>

const Symbol *LookupSymbol(const char *name) {
	u64 symbolIndex;

	for (symbolIndex = 0; symbolIndex < symbolCount; symbolIndex++) {
		if (Strncmp(symbols[symbolIndex].name, name, MAX_SYMBOL_LENGTH) == 0) return &symbols[symbolIndex - 1];
	}

	return NULL;

}

const Symbol *LookupSymbol(u64 addr) {
	u64 symbolIndex;

	if (addr < symbols[0].addr || addr > symbols[symbolCount - 1].addr) return NULL;

	for (symbolIndex = 0; symbolIndex < symbolCount; symbolIndex++) {
		if (symbols[symbolIndex].addr > addr) break;
	}

	return &symbols[symbolIndex - 1];
}
