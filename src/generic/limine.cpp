#include <cdefs.h>
#include <cstdint.hpp>
#include <limine.h>
#include <panic.hpp>
#include <main.hpp>
#include <boot.hpp>
#include <kinfo.hpp>
#include <bootmem.hpp>
#include <string.hpp>
#include <printk.hpp>
#include <pmm.hpp>

LIMINE_BASE_REVISION(1)

/* Limine will not map memory over 4G */
#define LIMINE_MEMORY_MAP_LIMIT (0xFFFFFFFF)

/* Function called by Limine at bootup through the entry point */
extern "C" void LimineEntry(void);

/* Setting the correct entry point for Limine */
static volatile limine_entry_point_request EntryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.response = NULL,
	.entry = &LimineEntry
};

/* Module request */
static volatile limine_module_request ModuleRequest {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 0,
	.response = NULL,
	.internal_module_count = 0,
	.internal_modules = 0
};

/* High half direct mapping start request */
static volatile limine_hhdm_request HHDMRequest {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
	.response = NULL
};

/* Paging mode request */
static volatile limine_paging_mode_request PagingModeRequest {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.response = NULL,
	.mode = LIMINE_PAGING_MODE_DEFAULT,
	.flags = 0
};

/* Memory map request */
static volatile limine_memmap_request MemoryMapRequest {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
	.response = NULL
};

/* Kernel address request */
static volatile limine_kernel_address_request KAddrRequest {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0,
	.response = NULL
};

/* RSDP request */
static volatile limine_rsdp_request RSDPRequest {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0,
	.response = NULL
};

/* DTB Request */
static volatile limine_dtb_request DTBRequest {
	.id = LIMINE_DTB_REQUEST,
	.revision = 0,
	.response = NULL
};

/* Main Limine initialization function */
extern "C" __attribute__((noreturn))
void LimineEntry() {
	/* Checking if vital requests have been answered by Limine
	 * If not, give up and shut down */
	if(MemoryMapRequest.response == NULL ||
	   HHDMRequest.response == NULL ||
	   KAddrRequest.response == NULL) {
		PANIC("Requests not answered by Limine");
	}

	InitInfo();
	
	KInfo *info = GetInfo();

	info->HigherHalfMapping = HHDMRequest.response->offset;
	info->KernelPhysicalBase = KAddrRequest.response->physical_base;
	info->KernelVirtualBase = KAddrRequest.response->virtual_base;

	uptr bootmemRegionBase = 0;
	usize bootmemRegionLength = 0;

	for (usize i = 0; i < MemoryMapRequest.response->entry_count; i++) {
		uptr base = MemoryMapRequest.response->entries[i]->base;
		usize length = MemoryMapRequest.response->entries[i]->length;
		u8 type = MemoryMapRequest.response->entries[i]->type;

		/* Exclude invalid entries */
		if (type != MEMMAP_USABLE ||
		    base >= LIMINE_MEMORY_MAP_LIMIT ||
		    base + length >= LIMINE_MEMORY_MAP_LIMIT) {
			continue;
		}

		/* We also do the equals because it's preferable to have
		 * a higher memory region (that are abundant) compared to
		 * low memory regions (i.e. below 64MB)
		 */
		if (length >= bootmemRegionLength) {
			bootmemRegionBase = base;
			bootmemRegionLength = length;
		}
	}

	bootmemRegionLength = BOOTMEM::ActivateBootMemory(bootmemRegionBase + info->HigherHalfMapping, bootmemRegionLength);
	
	/* Transporting the MMAP */
	PRINTK::PrintK(PRINTK_DEBUG "Get the memory map from Limine.\r\n");

	MEM::MEMBLOCK::MemblockAllocator *alloc = MEM::MEMBLOCK::InitializeAllocator();

	for (usize i = 0; i < MemoryMapRequest.response->entry_count; i++) {
		uptr base = MemoryMapRequest.response->entries[i]->base;
		usize length = MemoryMapRequest.response->entries[i]->length;
		u8 type = MemoryMapRequest.response->entries[i]->type;

		if (type == MEMMAP_RESERVED) continue;

		MEM::MEMBLOCK::AddRegion(alloc, base, length, type);
	}

	info->PhysicalMemoryChunks = alloc;

	/* Setting all the memory used by BOOTMEM as used */
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks,
			bootmemRegionBase, bootmemRegionLength, MEMMAP_KERNEL_AND_MODULES);


	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator(LIMINE_MEMORY_MAP_LIMIT);


	/* Transporting files 
	const char *cmdline = KernelFileRequest.response->kernel_file->cmdline;
	usize len = Strnlen(cmdline, MAX_CMDLINE_LENGTH);
	info->KernelArgs = (const char*)BOOTMEM::Malloc(len + 1);
	Memcpy((u8*)info->KernelArgs, (u8*)cmdline, len);
	*(char*)&info->KernelArgs[len] = '\0';

	if(ModuleRequest.response != NULL) {
		int moduleCount = ModuleRequest.response->module_count;
		info->FileCount = moduleCount;

		info->BootFiles = (BootFile*)BOOTMEM::Malloc(moduleCount * sizeof(BootFile));

		PRINTK::PrintK(PRINTK_DEBUG "Allocating for %d modules.\r\n", moduleCount);

		for (int i = 0; i < moduleCount; i++) {
			usize fileSize = ModuleRequest.response->modules[i]->size;
			ROUND_UP_TO_PAGE(fileSize);

			MEM::MEMBLOCK::AddRegion(alloc,
				(uptr)ModuleRequest.response->modules[i]->address - info->HigherHalfMapping,
				fileSize,
				MEMMAP_KERNEL_FILE);

			info->BootFiles[i].Address = (uptr)ModuleRequest.response->modules[i]->address;
			info->BootFiles[i].Size = ModuleRequest.response->modules[i]->size;
			Strncpy(info->BootFiles[i].Path, ModuleRequest.response->modules[i]->path, MAX_FILE_NAME_LENGTH);
		
			Strncpy(info->BootFiles[i].Cmdline, ModuleRequest.response->modules[i]->cmdline, MAX_FILE_NAME_LENGTH);
		}
	} else {
		info->FileCount = 0;
		info->BootFiles = NULL;
	}*/

	if(RSDPRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: no RSDP found.\r\n");
	} else {
		if(RSDPRequest.response->address != NULL) {
			PRINTK::PrintK(PRINTK_DEBUG "RSDP found at 0x%x\r\n", RSDPRequest.response->address);
			info->RSDP = (uptr)RSDPRequest.response->address;
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "WARNING: no RSDP found.\r\n");
		}
	}

	if(DTBRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: No DTB found.\r\n");
	} else {
		if(DTBRequest.response->dtb_ptr != NULL) {
			PRINTK::PrintK(PRINTK_DEBUG "DTB found at 0x%x\r\n", DTBRequest.response->dtb_ptr);
			info->DeviceTree = (uptr)DTBRequest.response->dtb_ptr;
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "WARNING: No DTB found.\r\n");
		}
	}

	/* Launch the kernel proper */
	KernelStart();

	while(true);
}
