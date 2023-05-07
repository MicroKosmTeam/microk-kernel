#pragma once
#include <stdint.h>
#include <stddef.h>

enum ELFType {
	ELF_MKMI,
	ELF_CORE_MODULE,
};

uint64_t LoadELF(ELFType type, uint8_t *data, size_t size);
