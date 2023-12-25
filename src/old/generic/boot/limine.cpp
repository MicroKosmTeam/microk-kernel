#include <cdefs.h>
#include <cstdint.hpp>

#include <sys/locks.hpp>
#include <boot/limine.h>
#include <sys/panic.hpp>
#include <init/main.hpp>
#include <boot/boot.hpp>
#include <init/kinfo.hpp>
#include <mm/bootmem.hpp>
#include <mm/string.hpp>
#include <sys/printk.hpp>
#include <sys/arch.hpp>

LIMINE_BASE_REVISION(1)

/* Function called by Limine at bootup through the entry point */
extern "C" void LimineEntry(void);

/* Setting the correct entry point for Limine */
static volatile limine_entry_point_request EntryPointRequest {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.response = NULL,
	.entry = &LimineEntry
};

/* Stack request */
/* Stack size configured in autoconf.h */
static volatile limine_stack_size_request StackRequest {
	.id = LIMINE_STACK_SIZE_REQUEST,
	.revision = 0,
	.response = NULL,
	.stack_size = CONFIG_STACK_SIZE
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

/* SMBIOS request */
static volatile limine_smbios_request SMBIOSRequest {
	.id = LIMINE_SMBIOS_REQUEST,
	.revision = 0,
	.response = NULL
};

/* EFI system table request */
static volatile limine_efi_system_table_request EFITableRequest {
	.id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
	.revision = 0,
	.response = NULL
};

/* Framebuffer request */
static volatile limine_framebuffer_request FramebufferRequest {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0,
	.response = NULL
};

static volatile limine_kernel_file_request KernelFileRequest {
	.id = LIMINE_KERNEL_FILE_REQUEST,
	.revision = 0,
	.response = NULL
};


/* DTB Request */
static volatile limine_dtb_request DTBRequest {
	.id = LIMINE_DTB_REQUEST,
	.revision = 0,
	.response = NULL
};


static volatile limine_boot_time_request TimeRequest {
	.id = LIMINE_BOOT_TIME_REQUEST,
	.revision = 0,
	.response = NULL
};

extern uptr __stack_chk_guard;

/* Main Limine initialization function */
extern "C" __attribute__((no_stack_protector)) __attribute__((noreturn))
void LimineEntry() {
	if(TimeRequest.response != NULL) {
		#if UINT32_MAX == UINTPTR_MAX
			__stack_chk_guard = TimeRequest.response->boot_time & 0xFFFFFFFF;
		#else
			__stack_chk_guard = TimeRequest.response->boot_time;
		#endif
	} else {
		#if UINT32_MAX == UINTPTR_MAX
			__stack_chk_guard = 0xe2dee396;
		#else
			__stack_chk_guard = 0x595e9fbd94fda766;
		#endif
	}

	InitInfo();
	BOOTMEM::ActivateBootmem();

	/* Checking if vital requests have been answered by Limine
	 * If not, give up and shut down */
	if(StackRequest.response == NULL ||
	   MemoryMapRequest.response == NULL ||
	   HHDMRequest.response == NULL ||
	   KAddrRequest.response == NULL ||
	   KernelFileRequest.response == NULL
	   ) {
		PANIC("Requests not answered by Limine");
	}

	
	KInfo *info = GetInfo();

	uptr stackPtr = ARCH::GetCurrentStackPointer();

	ROUND_UP_TO_PAGE(stackPtr);
	PRINTK::PrintK(PRINTK_DEBUG "Limine allocated kernel stack: 0x%x\r\n", stackPtr);
	
	/* Transporting the MMAP */
	PRINTK::PrintK(PRINTK_DEBUG "Get the memory map from Limine.\r\n");

	info->HigherHalfMapping = HHDMRequest.response->offset;
	info->KernelPhysicalBase = KAddrRequest.response->physical_base;
	info->KernelVirtualBase = KAddrRequest.response->virtual_base;

	MEM::MEMBLOCK::MemblockAllocator *alloc = MEM::MEMBLOCK::InitializeAllocator();

	for (usize i = 0; i < MemoryMapRequest.response->entry_count; i++) {
		uptr base = MemoryMapRequest.response->entries[i]->base;
		usize length = MemoryMapRequest.response->entries[i]->length;
		u8 type = MemoryMapRequest.response->entries[i]->type;

		if (type == MEMMAP_BOOTLOADER_RECLAIMABLE &&
		    base + length == stackPtr - info->HigherHalfMapping) {
			type = MEMMAP_KERNEL_STACK;
		}
		
		MEM::MEMBLOCK::AddRegion(alloc, base, length, type);

	}

	info->PhysicalMemoryChunks = alloc;

	info->KernelFileAddress = (uptr)KernelFileRequest.response->kernel_file->address;
	info->KernelFileSize = (uptr)KernelFileRequest.response->kernel_file->size;

	const char *cmdline = KernelFileRequest.response->kernel_file->cmdline;
	usize len = Strnlen(cmdline, MAX_CMDLINE_LENGTH);
	info->KernelArgs = (const char*)BOOTMEM::Malloc(len + 1);
	Memcpy((u8*)info->KernelArgs, (u8*)cmdline, len);
	*(char*)&info->KernelArgs[len] = '\0';

	/* Transporting files */
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
	}

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

	if(SMBIOSRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: no viable SMBIOS entry found.\r\n");
	} else {
		if(SMBIOSRequest.response->entry_64 != NULL) {
			PRINTK::PrintK(PRINTK_DEBUG "SMBIOS 64 entry at 0x%x\r\n", SMBIOSRequest.response->entry_64);
		} else if (SMBIOSRequest.response->entry_32 != NULL) {
			PRINTK::PrintK(PRINTK_DEBUG "SMBIOS 32 entry at 0x%x\r\n", SMBIOSRequest.response->entry_32);
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "WARNING: no viable SMBIOS entry found.\r\n");
		}
	}

	if(EFITableRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: EFI system table not found, assuming we're running on a BIOS system.\r\n");
	} else {
		if(EFITableRequest.response->address != NULL) {
			PRINTK::PrintK(PRINTK_DEBUG "EFI system table found at 0x%x\r\n", EFITableRequest.response->address);
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "WARNING: EFI system table not found, assuming we're running on a BIOS system.\r\n");
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
	
	if (FramebufferRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: No framebuffers detected.\r\n");
		info->FramebufferCount = 0;
		info->Framebuffers = NULL;
	} else {
		int framebufferCount = FramebufferRequest.response->framebuffer_count;
		PRINTK::PrintK(PRINTK_DEBUG "%d framebuffers detected.\r\n", framebufferCount);
		info->FramebufferCount = framebufferCount;
		info->Framebuffers = (Framebuffer*)BOOTMEM::Malloc(sizeof(Framebuffer) * framebufferCount);

		for (int i = 0; i < framebufferCount; i++) {
			info->Framebuffers[i].Address = FramebufferRequest.response->framebuffers[i]->address;
			info->Framebuffers[i].Width = FramebufferRequest.response->framebuffers[i]->width;
			info->Framebuffers[i].Height = FramebufferRequest.response->framebuffers[i]->height;
			info->Framebuffers[i].BPP = FramebufferRequest.response->framebuffers[i]->bpp;
			info->Framebuffers[i].RedShift = FramebufferRequest.response->framebuffers[i]->red_mask_shift;
			info->Framebuffers[i].GreenShift = FramebufferRequest.response->framebuffers[i]->green_mask_shift;
			info->Framebuffers[i].BlueShift = FramebufferRequest.response->framebuffers[i]->blue_mask_shift;
		}
	}

	/* Launch the kernel proper */
	KernelStart();

	while(true);
}