#include <cdefs.h>
#include <limine.h>
#include <sys/panic.hpp>
#include <init/main.hpp>
#include <boot/boot.hpp>
#include <init/kinfo.hpp>
#include <mm/bootmem.hpp>
#include <sys/printk.hpp>

/* Function called by Limine at bootup through the entry point */
extern "C" void LimineEntry(void);

/* Setting the correct entry point for Limine */
static volatile limine_entry_point_request entryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.entry = &LimineEntry
};

/* System time request */
static volatile limine_boot_time_request timeRequest {
	.id = LIMINE_BOOT_TIME_REQUEST,
	.revision = 0
};

/* Bootloader info request */
static volatile limine_bootloader_info_request bootloaderRequest {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0
};

/* Stack request */
/* Stack size configured in autoconf.h */
static volatile limine_stack_size_request stackRequest {
	.id = LIMINE_STACK_SIZE_REQUEST,
	.revision = 0,
	.stack_size = CONFIG_STACK_SIZE
};

/* Module request */
static volatile limine_module_request moduleRequest {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 0
};

/* High half direct mapping start request */
static volatile limine_hhdm_request hhdmRequest {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0
};

/* Memory map request */
static volatile limine_memmap_request mMapRequest {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0
};

/* Kernel address request */
static volatile limine_kernel_address_request kaddrRequest {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0
};

/* RSDP request */
static volatile limine_rsdp_request rsdpRequest {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0
};

/* Framebuffer request */
static volatile limine_framebuffer_request framebufferRequest {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

/* Main Limine initialization function */
__attribute__((noreturn)) extern "C" void LimineEntry() {
	/* Startup basic kernel runtime services */
	EarlyKernelInit();

	KInfo *info = GetInfo();

	/* Checking if vital requests have been answered by Limine */
	/* If not, give up and shut down */
	if(timeRequest.response == NULL ||
	   stackRequest.response == NULL ||
	   bootloaderRequest.response == NULL ||
	   moduleRequest.response == NULL ||
	   mMapRequest.response == NULL ||
	   hhdmRequest.response == NULL ||
	   kaddrRequest.response == NULL
	   )
		PANIC("Requests not answered by Limine");

	/* Transporting the MMAP */
	PRINTK::PrintK("Getting the memory map from Limine...\r\n");

	uint64_t mMapEntryCount = mMapRequest.response->entry_count;
	info->mMapEntryCount = mMapEntryCount;
	info->mMap = (MEM::MMapEntry*)BOOTMEM::Malloc(sizeof(MEM::MMapEntry) * mMapEntryCount + 1);

	PRINTK::PrintK("Allocating for %d memory map entries.\r\n", mMapEntryCount);
	for (int i = 0; i < mMapEntryCount; i++) {
		info->mMap[i].base = mMapRequest.response->entries[i]->base;
		info->mMap[i].length = mMapRequest.response->entries[i]->length;
		info->mMap[i].type = mMapRequest.response->entries[i]->type;
	}

	info->higherHalfMapping = hhdmRequest.response->offset;
	info->kernelPhysicalBase = kaddrRequest.response->physical_base;
	info->kernelVirtualBase = kaddrRequest.response->virtual_base;

	/* Transporting files */
	uint64_t moduleCount = moduleRequest.response->module_count;
	info->fileCount = moduleCount;
	info->bootFiles = (FILE::BootFile*)BOOTMEM::Malloc(sizeof(FILE::BootFile) * moduleCount + 1);

	PRINTK::PrintK("Allocating for %d modules.\r\n", moduleCount);
	for (int i = 0; i < moduleCount; i++) {
		info->bootFiles[i].Address = moduleRequest.response->modules[i]->address;
		info->bootFiles[i].Size = moduleRequest.response->modules[i]->size;
		info->bootFiles[i].Path = moduleRequest.response->modules[i]->path;
		info->bootFiles[i].Cmdline = moduleRequest.response->modules[i]->cmdline;
	}

	if (rsdpRequest.response == NULL) {
		info->RSDP = NULL;
	} else {
		info->RSDP = rsdpRequest.response->address;
	}

	if (framebufferRequest.response == NULL) {
		info->framebufferCount = 0;
		info->framebuffers = NULL;
	} else {
		size_t framebufferCount = framebufferRequest.response->framebuffer_count;
		info->framebufferCount = framebufferCount;
		info->framebuffers = (Framebuffer*)BOOTMEM::Malloc(sizeof(Framebuffer) * framebufferCount + 1);

		for (int i = 0; i < framebufferCount; i++) {
			info->framebuffers[i].Address = framebufferRequest.response->framebuffers[i]->address;
			info->framebuffers[i].Width = framebufferRequest.response->framebuffers[i]->width;
			info->framebuffers[i].Height = framebufferRequest.response->framebuffers[i]->height;
			info->framebuffers[i].BPP = framebufferRequest.response->framebuffers[i]->bpp;
			info->framebuffers[i].RedShift = framebufferRequest.response->framebuffers[i]->red_mask_shift;
			info->framebuffers[i].GreenShift = framebufferRequest.response->framebuffers[i]->green_mask_shift;
			info->framebuffers[i].BlueShift = framebufferRequest.response->framebuffers[i]->blue_mask_shift;
		}
	}

	PRINTK::PrintK("Welcome from MicroK.\r\n"
		       "The kernel is booted by %s %s at %d\r\n",
		       bootloaderRequest.response->name,
		       bootloaderRequest.response->version,
		       timeRequest.response->boot_time);

	/* Launch the kernel proper */
	KernelStart();

	while(true);
}
