#include <cdefs.h>
#include <limine.h>
#include <panic.hpp>
#include <main.hpp>
#include <boot.hpp>
#include <kinfo.hpp>
#include <bootmem.hpp>
#include <string.hpp>
#include <printk.hpp>
#include <pmm.hpp>

static volatile LIMINE_BASE_REVISION(1)

/* Function called by Limine at bootup through the entry point */
extern "C" void LimineEntry();

/* Setting the correct entry point for Limine */
static volatile limine_entry_point_request EntryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.response = NULL,
	.entry = &LimineEntry
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

/* Forcing the bootloader to provide the user manager */
static volatile limine_internal_module UserManagerModule {
	.path = "./" USER_MANAGER_NAME,
	.cmdline = NULL,
	.flags = LIMINE_INTERNAL_MODULE_REQUIRED,
};

static volatile limine_internal_module *InternalModules[] {
	&UserManagerModule,
};

/* Module request */
static volatile limine_module_request ModuleRequest {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 1,
	.response = NULL,
	.internal_module_count = 1,
	.internal_modules = (limine_internal_module**)InternalModules,
};

/* Main Limine initialization function */
extern "C" __attribute__((noreturn))
void LimineEntry() {
	/* Checking if vital requests have been answered by Limine
	 * If not, give up and shut down */
	if(limine_base_revision[2] != 0 ||
	   MemoryMapRequest.response == NULL ||
	   HHDMRequest.response == NULL ||
	   KAddrRequest.response == NULL) {
		PANIC("Requests not answered by Limine");
	}

	InitInfo();
	
	KInfo *info = GetInfo();

	info->HigherHalfMapping = HHDMRequest.response->offset;
	info->KernelPhysicalBase = KAddrRequest.response->physical_base;
	info->KernelVirtualBase = KAddrRequest.response->virtual_base;

	uptr largestRegionBase = 0;
	usize largestRegionLength = 0;

	for (usize i = 0; i < MemoryMapRequest.response->entry_count; i++) {
		uptr base = MemoryMapRequest.response->entries[i]->base;
		usize length = MemoryMapRequest.response->entries[i]->length;
		u8 type = MemoryMapRequest.response->entries[i]->type;

		/* Exclude invalid entries */
		if (type != MEMMAP_USABLE) {
			continue;
		}

		/* We also do the equals because it's preferable to have
		 * a higher memory region (that are abundant) compared to
		 * low memory regions (i.e. below 64MB)
		 */
		if (length >= largestRegionLength) {
			largestRegionBase = base;
			largestRegionLength = length;
		}
	}

	info->DesignedAllocationRegionBase = largestRegionBase;
	info->DesignedAllocationRegionLength = largestRegionLength;

	PMM::ValidateMemory();

	largestRegionLength = BOOTMEM::ActivateBootMemory(largestRegionBase + info->HigherHalfMapping, largestRegionLength);
	
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
			largestRegionBase, largestRegionLength, MEMMAP_KERNEL_BOOTMEM);


	info->ManagerExecutableAddress = (uptr)ModuleRequest.response->modules[0]->address;
	info->ManagerExecutableSize = ModuleRequest.response->modules[0]->size;

	PRINTK::PrintK(PRINTK_DEBUG "Manager executable at 0x%x is %d bytes\r\n", info->ManagerExecutableAddress, info->ManagerExecutableSize);

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
