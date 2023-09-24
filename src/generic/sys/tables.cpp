#include <sys/tables.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>

int InitializeKernelTables() {
	KInfo *info = GetInfo();

	info->KernelBaseSystemTable = (uintptr_t)PMM::RequestPage();
	KBST *kbst = (KBST*)(info->KernelBaseSystemTable + info->HigherHalfMapping);
	Memset(kbst, 0, PAGE_SIZE);
	PopulateKBST(kbst);

	info->BootFileSystemTable = (uintptr_t)PMM::RequestPage();
	BFST *bfst = (BFST*)(info->BootFileSystemTable + info->HigherHalfMapping);
	Memset(bfst, 0, PAGE_SIZE);
	PopulateBFST(bfst);

	return 0;
}

int UpdateKernelTables() {
	KInfo *info = GetInfo();
	KBST *kbst = (KBST*)(info->KernelBaseSystemTable + info->HigherHalfMapping);

	PopulateKBST(kbst);

	return 0;
}

void PopulateKBST(KBST *kbst) {
	kbst->Signature[0] = 'K';
	kbst->Signature[1] = 'B';
	kbst->Signature[2] = 'S';
	kbst->Signature[3] = 'T';
	kbst->Revision = 1;

	kbst->FreePhysicalMemory = PMM::GetFreeMem();
	kbst->UsedPhysicalMemory = PMM::GetUsedMem();
	kbst->ReservedPhysicalMemory = PMM::GetReservedMem();

	uint8_t checksumDifference = 0;
	uint8_t *kbstByte = (uint8_t*)kbst;

	while((uintptr_t)kbstByte < (uintptr_t)kbst + PAGE_SIZE) {
		checksumDifference += *kbstByte++;
	}

	kbst->Checksum = 0x100 - checksumDifference;
}

#include <mm/string.hpp>
void PopulateBFST(BFST *bfst) {
	KInfo *info = GetInfo();

	bfst->Signature[0] = 'B';
	bfst->Signature[1] = 'F';
	bfst->Signature[2] = 'S';
	bfst->Signature[3] = 'T';
	bfst->Revision = 1;

	bfst->NumberOfFiles = info->FileCount;
	
	Memcpy(bfst->Files, info->BootFiles, sizeof(BootFile) * info->FileCount);

	uint8_t checksumDifference = 0;
	uint8_t *bfstByte = (uint8_t*)bfst;

	while((uintptr_t)bfstByte < (uintptr_t)bfst + PAGE_SIZE) {
		checksumDifference += *bfstByte++;
	}

	bfst->Checksum = 0x100 - checksumDifference;
}
