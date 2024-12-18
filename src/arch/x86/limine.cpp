#include <cdefs.h>
#include <limine.h>
#include <panic.hpp>
#include <main.hpp>
#include <boot.hpp>
#include <kinfo.hpp>
#include <string.hpp>
#include <printk.hpp>
#include <capability.hpp>
#include <random.hpp>
#include <sha256.hpp>
#include <micro-ecc/uECC.h>
#include <slab.hpp>
#include <cpu.hpp>
#include <pmm.hpp>

static volatile LIMINE_BASE_REVISION(1);

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
	.max_mode = LIMINE_PAGING_MODE_DEFAULT,
	.min_mode = LIMINE_PAGING_MODE_DEFAULT
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

static volatile limine_framebuffer_request FramebufferRequest {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
    	.revision = 0,
	.response = NULL
};

/* Forcing the bootloader to provide the user manager */
static volatile limine_internal_module UserManagerModule {
	.path = "./" USER_MANAGER_NAME,
	.cmdline = NULL,
	.flags = LIMINE_INTERNAL_MODULE_REQUIRED,
};

/* Forcing the bootloader to provide the user manager */
static volatile limine_internal_module InitrdModule {
	.path = "./initrd.tar",
	.cmdline = NULL,
	.flags = LIMINE_INTERNAL_MODULE_REQUIRED,
};

static volatile limine_internal_module *InternalModules[] {
	&UserManagerModule,
	&InitrdModule,
};

/* Module request */
static volatile limine_module_request ModuleRequest {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 1,
	.response = NULL,
	.internal_module_count = 2,
	.internal_modules = (limine_internal_module**)InternalModules,
};


static volatile limine_boot_time_request BootTimeRequest {
	.id = LIMINE_BOOT_TIME_REQUEST,
	.revision = 0,
	.response = NULL,
};

static volatile limine_kernel_file_request KernelFileRequest {
	.id = LIMINE_KERNEL_FILE_REQUEST,
	.revision = 0,
	.response = NULL
};

/* Main Limine initialization function */
extern "C" __attribute__((noreturn))
void LimineEntry() {
	/* Checking if vital requests have been answered by Limine
	 * If not, give up and shut down */
	if(limine_base_revision[2] != 0 ||
	   MemoryMapRequest.response == NULL ||
	   HHDMRequest.response == NULL ||
	   KAddrRequest.response == NULL ||
	   BootTimeRequest.response == NULL ||
	   KernelFileRequest.response == NULL) {
		PANIC("Requests not answered by Limine");
	}

	InitInfo();
	
	KInfo *info = GetInfo();
	
	char *responseCmdline = KernelFileRequest.response->kernel_file->cmdline;
	usize length = Strnlen(responseCmdline, PAGE_SIZE);
	char cmdline[length + 1];
	Strncpy(cmdline, responseCmdline, length);
	cmdline[length] = '\0';
	info->KernelArgs = cmdline;
	PRINTK::PrintK(PRINTK_DEBUG "Kernel cmdline: %s\r\n", info->KernelArgs);

	ARCH::InitializeBootCPU();
	RANDOM::SRand(BootTimeRequest.response->boot_time);
	uECC_set_rng(reinterpret_cast<uECC_RNG_Function>((void*)&RANDOM::Rng));
	/*
	uECC_set_rng(reinterpret_cast<uECC_RNG_Function>((void*)&RANDOM::Rng));

	uECC_Curve curve = uECC_secp256k1();

	u8 privateKey[SECP256k1_PRIVATE_KEY_SIZE];
	u8 publicKey[SECP256k1_PUBLIC_KEY_SIZE];

	uECC_make_key(publicKey, privateKey, curve);

	EncryptedCapabilityContext context;
	
	uECC_shared_secret(publicKey, privateKey, context.Secret, curve);

	EncryptedCapability encryptedCap;
	Capability *capability = (Capability*)encryptedCap.CapabilityData;
	capability->Object = 0xDEADBEEF69420;

	{
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, encryptedCap.CapabilityData, sizeof(Capability));
		sha256_final(&ctx, encryptedCap.SHA256Hash);
	}
		
	PRINTK::PrintK(PRINTK_DEBUG "Hash generated!\r\n");

	RANDOM::Rng(encryptedCap.IV, 16);

	{
		AES_ctx encryptContext;
		AES_init_ctx_iv(&encryptContext, context.Secret, encryptedCap.IV);
		AES_CBC_encrypt_buffer(&encryptContext, encryptedCap.SHA256Hash, SHA256_BLOCK_SIZE);
	}
	
	PRINTK::PrintK(PRINTK_DEBUG "Encrypted!\r\n");

	{
		AES_ctx decryptContext;
		AES_init_ctx_iv(&decryptContext, context.Secret, encryptedCap.IV);
		AES_CBC_decrypt_buffer(&decryptContext, encryptedCap.SHA256Hash, SHA256_BLOCK_SIZE);
	}

	PRINTK::PrintK(PRINTK_DEBUG "Decrypted!\r\n");
	
	u8 checkHash[SHA256_BLOCK_SIZE];
	{
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, encryptedCap.CapabilityData, sizeof(Capability));
		sha256_final(&ctx, checkHash);
	}
	
	if (Memcmp(checkHash, encryptedCap.SHA256Hash, SHA256_BLOCK_SIZE) == 0) {
		PRINTK::PrintK(PRINTK_DEBUG "Hashes match!\r\n");
	} else {
		PRINTK::PrintK(PRINTK_DEBUG "Hashes dont match!\r\n");
	}
	
	PRINTK::PrintK(PRINTK_DEBUG "Encrypted capability is %d bytes\r\n", sizeof(EncryptedCapability));
	PRINTK::PrintK(PRINTK_DEBUG "Capability is %d bytes\r\n", sizeof(Capability));
	PRINTK::PrintK(PRINTK_DEBUG "CapabilityContext is %d bytes\r\n", sizeof(EncryptedCapabilityContext));
	PRINTK::PrintK(PRINTK_DEBUG "Capabilities per page: %d\r\n", PAGE_SIZE / sizeof(EncryptedCapability));

	while(true) {}
	*/

	info->HigherHalfMapping = HHDMRequest.response->offset;
	info->KernelPhysicalBase = KAddrRequest.response->physical_base;
	info->KernelVirtualBase = KAddrRequest.response->virtual_base;
		

	usize memoryRegions = MemoryMapRequest.response->entry_count;
	MemoryHeader memoryMap[memoryRegions + 1];

	PRINTK::PrintK(PRINTK_DEBUG "Memory regions from bootloader: %d\r\n", memoryRegions);

	info->MemoryMap = memoryMap;

	info->ManagerExecutableAddress = (uptr)ModuleRequest.response->modules[0]->address;
	info->ManagerExecutableSize = ModuleRequest.response->modules[0]->size;
	info->InitrdAddress = (uptr)ModuleRequest.response->modules[1]->address;
	info->InitrdSize = ModuleRequest.response->modules[1]->size;

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

	if(FramebufferRequest.response == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "WARNING: No framebuffer found.\r\n");
	} else {
		if(FramebufferRequest.response->framebuffer_count != 0) {
			PRINTK::PrintK(PRINTK_DEBUG "%d framebuffer(s) found\r\n", FramebufferRequest.response->framebuffer_count);
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "WARNING: No framebuffer found.\r\n");
		}
	}

	{
		MemoryHeader *longestRegion = NULL;

		usize longestRegionLength = 0;
		
		for (usize i = 0; i < memoryRegions; i++) {
			uptr base = MemoryMapRequest.response->entries[i]->base;
			usize length = MemoryMapRequest.response->entries[i]->length;
			u8 type = MemoryMapRequest.response->entries[i]->type;
		
			memoryMap[i].Address = base;
			memoryMap[i].Length = length;
			memoryMap[i].Flags = type;

			/* Exclude invalid entries */
			if (type != MEMMAP_USABLE) {
				continue;
			}

			/* We also do the equals because it's preferable to have
			 * a higher memory region (that are abundant) compared to
			 * low memory regions (i.e. below 64MB)
			 */
			if (length >= longestRegionLength) { //&& base < ((uptr)4 * 1024 * 1024 * 1024)) {
				longestRegionLength = length;
				longestRegion = &memoryMap[i];
			}
		}
		
		memoryMap[memoryRegions].Address = -1;

		uptr addr = CAPABILITY::InitializeRootSpace(longestRegion->Address, memoryMap);

		PRINTK::PrintK(PRINTK_DEBUG "Memory map now:\r\n");
		for (usize i = 0; i < memoryRegions; i++) {	
			PRINTK::PrintK(PRINTK_DEBUG "0x%x %dbytes of %d type\r\n", memoryMap[i].Address, memoryMap[i].Length, memoryMap[i].Flags);
		}

		Capability *cap = CAPABILITY::AddressCapability(info->RootCSpace, addr);
		PMM::Init(cap);

		ContainerInfo *cinfo = (ContainerInfo*)PMM::RequestPage();
		cinfo->InitrdAddress = VMM::VirtualToPhysical(info->InitrdAddress);
		cinfo->InitrdSize = info->InitrdSize;
		cinfo->x86_64.RSDPCapability = VMM::VirtualToPhysical(info->RSDP);
		cinfo->x86_64.RSDPOffset = cinfo->x86_64.RSDPCapability % PAGE_SIZE;
		ROUND_DOWN_TO_PAGE(cinfo->x86_64.RSDPCapability);
		cinfo->DTB = VMM::VirtualToPhysical(info->DeviceTree);
		info->_ContainerInfo = cinfo;

		/*
		for (int i = 0; i < 100; ++i) {
			PRINTK::PrintK(PRINTK_DEBUG "Requested page: 0x%x\r\n", PMM::RequestPage());
		}

		CAPABILITY::DumpCapabilitySlab(info->RootCSpace, UNTYPED);
		SLAB::Dump(info->RootCSpace->Slabs[UNTYPED].CapabilityTree);

		CAPABILITY::DumpCapabilitySlab(info->RootCSpace, THREAD_CONTROL_BLOCK);
		SLAB::Dump(info->RootCSpace->Slabs[THREAD_CONTROL_BLOCK].CapabilityTree);
	
		CAPABILITY::DumpCapabilitySlab(info->RootCSpace, CNODE);
		SLAB::Dump(info->RootCSpace->Slabs[CNODE].CapabilityTree);

		CAPABILITY::DumpCapabilitySlab(info->RootCSpace, CSPACE);
		SLAB::Dump(info->RootCSpace->Slabs[CSPACE].CapabilityTree);
		struct fdt_header {
			uint32_t magic;            // magic word FDT_MAGIC
			uint32_t totalsize;        // total size of DTB
			uint32_t off_dt_struct;    // offset to structure block
			uint32_t off_dt_strings;   // offset to strings block
			uint32_t off_mem_rsvmap;   // offset to memory reserve map
			uint32_t version;          // version of the DTB format
			uint32_t last_comp_version;// last compatible version
			uint32_t boot_cpuid_phys;  // physical ID of boot CPU
			uint32_t size_dt_strings;  // size of the strings block
			uint32_t size_dt_struct;   // size of the structure block
		};

		volatile fdt_header *dtb = (volatile fdt_header*)VMM::PhysicalToVirtual(memoryMap[0].Address);
		if(dtb->magic == 0xd00dfeed) {
			PRINTK::PrintK(PRINTK_DEBUG "Good\r\n");
		} else {
			PRINTK::PrintK(PRINTK_DEBUG "Bad\r\n");
		}*/

		// Testing
		/*
		
		usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
		PRINTK::PrintK(PRINTK_DEBUG "Slots: %d\r\n", slots);

		Capability *cap = CAPABILITY::AddressCapability(info->RootCSpace, address, UNTYPED);
		PRINTK::PrintK(PRINTK_DEBUG "Capability with address 0x%x: 0x%x\r\n", address, cap);
		MemoryHeader *header = (MemoryHeader*)cap->Object;
		PRINTK::PrintK(PRINTK_DEBUG "Header with address 0x%x: %d bytes\r\n", header->Address, header->Length);

		usize splitCount = 2;
		usize splitSize = PAGE_SIZE * 16;
		Capability *splitArray[splitCount];
		cap = CAPABILITY::SplitUntyped(info->RootCSpace, cap, splitSize, splitCount, splitArray);

		for (usize i = 0; i < splitCount; ++i) {
			PRINTK::PrintK(PRINTK_DEBUG "Capability with address 0x%x: 0x%x\r\n", splitArray[i]->Object, splitArray[i]);
			header = (MemoryHeader*)splitArray[i]->Object;
			PRINTK::PrintK(PRINTK_DEBUG "Header with address 0x%x: %d bytes\r\n", header->Address, header->Length);
		}
		

		PRINTK::PrintK(PRINTK_DEBUG "Capability with address 0x%x: 0x%x\r\n", cap->Object, cap);
		header = (MemoryHeader*)cap->Object;
		PRINTK::PrintK(PRINTK_DEBUG "Header with address 0x%x: %d bytes\r\n", header->Address, header->Length);

		slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
		PRINTK::PrintK(PRINTK_DEBUG "Slots: %d\r\n", slots);

		usize retypeCount = 16;
		Capability *frameRetypeArray[retypeCount];
		cap = CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAMES, retypeCount, frameRetypeArray);

		for (usize i = 0; i < retypeCount; ++i) {
			PRINTK::PrintK(PRINTK_DEBUG "Capability with address 0x%x: 0x%x\r\n", frameRetypeArray[i]->Object, frameRetypeArray[i]);
		}

		Capability *cnodeRetypeArray[retypeCount];
		cap = CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[1], CNODE, retypeCount, cnodeRetypeArray);

		for (usize i = 0; i < retypeCount; ++i) {
			PRINTK::PrintK(PRINTK_DEBUG "Capability with address 0x%x: 0x%x\r\n", cnodeRetypeArray[i]->Object, cnodeRetypeArray[i]);
			CAPABILITY::AddSlabNode(info->RootCSpace, UNTYPED, cnodeRetypeArray[i]);
		}

		slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
		PRINTK::PrintK(PRINTK_DEBUG "Slots: %d\r\n", slots);

		*/
	}

	/* Launch the kernel proper */
	KernelStart();

	while(true);
}
