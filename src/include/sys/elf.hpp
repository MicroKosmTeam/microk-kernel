#pragma once
#include <cstdint.hpp>
#include <debug/symbol.hpp>

usize LoadELF(u8 *data, usize size);
SYMBOL::SymbolPair *ExportSymbolTable(u8 *data, usize size);
