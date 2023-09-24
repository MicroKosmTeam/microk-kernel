#pragma once
#include <stdint.h>
#include <stddef.h>
#include <sys/file.hpp>

struct TableHeader {
	uint8_t Signature[4];
	uint8_t Revision;

	uint8_t Checksum;
}__attribute__((packed));

struct TableListElement {
	uint8_t Signature[4];
	uintptr_t TablePointer;
}__attribute__((packed));

struct KBST : public TableHeader {
	size_t FreePhysicalMemory;
	size_t UsedPhysicalMemory;
	size_t ReservedPhysicalMemory;
}__attribute__((packed));

struct PTST: public TableHeader {
}__attribute__((packed));

struct BFST: public TableHeader {
	size_t NumberOfFiles;
	BootFile Files[];
}__attribute__((packed));

int InitializeKernelTables();
int UpdateKernelTables();

void PopulateKBST(KBST *kbst);
void PopulateBFST(BFST *bfst);
