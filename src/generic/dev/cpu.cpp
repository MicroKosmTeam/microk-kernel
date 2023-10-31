#include <dev/cpu.hpp>
#include <sys/printk.hpp>
#include <mm/bootmem.hpp>

#if defined(ARCH_x64)
#include <arch/x64/main.hpp>
#include <arch/x64/cpu/cpu.hpp>
#elif defined(ARCH_aarch64)
#include <arch/aarch64/main.hpp>
#endif

namespace DEV::CPU {
TopologyStructure *CreateTopologyStructure(TopologyStructure *parent, u32 id) {
	TopologyStructure *structure = new TopologyStructure;

	structure->ID = id;
	structure->Parent = parent;
	structure->Children.First = structure->Children.Last = NULL;

	TopologyStructure *oldLastChild = parent->Children.Last;
	oldLastChild->Next = structure;
	structure->Previous = oldLastChild;
	parent->Children.Last = structure;

	return structure;
}

int InitializeBootCPU() {
	TopologyStructure *defaultMachine = (TopologyStructure*)BOOTMEM::Malloc(sizeof(TopologyStructure));
	TopologyStructure *defaultCluster = (TopologyStructure*)BOOTMEM::Malloc(sizeof(TopologyStructure));
	TopologyStructure *defaultPackage = (TopologyStructure*)BOOTMEM::Malloc(sizeof(TopologyStructure));
	TopologyStructure *defaultCore = (TopologyStructure*)BOOTMEM::Malloc(sizeof(TopologyStructure));
	TopologyStructure *defaultThread = (TopologyStructure*)BOOTMEM::Malloc(sizeof(TopologyStructure));

	defaultMachine->ID = 0;
	defaultMachine->Parent = NULL;
	defaultMachine->Next = NULL;
	defaultMachine->Children.First = defaultMachine->Children.Last = defaultCluster;

	defaultCluster->ID = 0;
	defaultCluster->Parent = defaultMachine;
	defaultCluster->Next = NULL;
	defaultCluster->Children.First = defaultCluster->Children.Last = defaultPackage;

	defaultPackage->ID = 0;
	defaultPackage->Parent = defaultCluster;
	defaultPackage->Next = NULL;
	defaultPackage->Children.First = defaultPackage->Children.Last = defaultCore;

	defaultCore->ID = 0;
	defaultCore->Parent = defaultPackage;
	defaultCore->Next = NULL;
	defaultCore->Children.First = defaultCore->Children.Last = defaultPackage;

	defaultThread->ID = 0;
	defaultThread->Parent = defaultCore;
	defaultThread->Next = NULL;
	defaultThread->Children.First = defaultThread->Children.Last = NULL;

	/* Initializing early architecture-specific devices */
#if defined(ARCH_x64)
	x86_64::BootCPUInit(defaultCore);
#endif
	return 0;
}

int SetupArch() {
#if defined(ARCH_x64)
	x86_64::Init();
#elif defined(ARCH_aarch64)
	AArch64::Init();
#endif

	return 0;
}

}
