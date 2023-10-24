#pragma once
#include <cstdint.hpp>

#include <sys/file.hpp>

struct TableHeader {
	u8 Signature[4];
	u8 Revision;

	u8 Checksum;
}__attribute__((packed));

struct TableListElement {
	u8 Signature[4];
	uptr TablePointer;
}__attribute__((packed));

struct KBST : public TableHeader {
	/* Memory data */
	usize FreePhysicalMemory;
	usize UsedPhysicalMemory;
	usize ReservedPhysicalMemory;

	/* Firmware data */
	uptr RSDP;
	uptr DeviceTree;
}__attribute__((packed));

struct PTST: public TableHeader {
}__attribute__((packed));

struct BFST: public TableHeader {
	usize NumberOfFiles;
	BootFile Files[];
}__attribute__((packed));

int InitializeKernelTables();
int UpdateKernelTables();

void PopulateKBST(KBST *kbst);
void PopulateBFST(BFST *bfst);

void UpdateKBST(KBST *kbst);
