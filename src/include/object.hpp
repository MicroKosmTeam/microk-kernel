#pragma once
#include <cdefs.h>
#include <sha256.hpp>
#include <tiny-aes/aes.hpp>

#if defined(__x86_64__)
#include <arch/x86/object.hpp>
#elif defined(__aarch64__)
#include <arch/aarch64/object.hpp>
#elif defined(__riscv) 
#include <arch/riscv64/object.hpp>
#endif

/*
 *
 */
typedef uptr CPtr;
typedef uptr VirtualSpace;

/* Structure that is the parent of all objects
 * that will belong in a linked list
 */
struct ListHead {
	ListHead *Next, *Previous;
};

/* Object that encompasses a linked list
 */
struct List {
	ListHead *Head;
	ListHead *Tail;
};

/*
 *
 */
struct Capability {
	u8 IsMasked : 1;
	u8 Type : 7;

	// TODO: overhaul parent/child relationship

	u16 AccessRights;
	u16 AccessRightsMask;

	union {
		uptr Object;
		uptr IOStart;
		usize TimesliceStart;
	};
	usize Size;
}__attribute__((packed));

struct CapabilityTreeNode : public Capability {
	CapabilityTreeNode *Left, *Right;

	u32 Level;
}__attribute__((packed));



/*
 * WARNING: Experimental, for future use
 */
struct EncryptedCapability {
	u8 CapabilityData[sizeof(Capability)];
	u8 SHA256Hash[SHA256_BLOCK_SIZE]; /* Encrypted hash */
	u8 IV[AES_BLOCKLEN];
	/* IV, will be regenerated each time the
	   hash reencrypted */
}__attribute__((aligned(0x10)));

#define SECP256k1_PRIVATE_KEY_SIZE 32
#define SECP256k1_PUBLIC_KEY_SIZE 64
#define SECP256k1_SHARED_SECRET_SIZE 32
struct EncryptedCapabilityContext {
	u8 Secret[SECP256k1_SHARED_SECRET_SIZE];
}__attribute__((aligned(0x10)));

/*
 *
 */
struct MemoryHeader {
	uptr Address;
	usize Length;
	u32 Flags;
}__attribute__((packed));


/*
 *
 */

struct SlabHead : public ListHead {
	u16 FreeElements;
};

#define CAPABILITIES_PER_NODE ((PAGE_SIZE - sizeof(SlabHead)) / sizeof(CapabilityTreeNode))

/*
 *
 */
struct CapabilityNode : public SlabHead {
	CapabilityTreeNode Slots[CAPABILITIES_PER_NODE];
}__attribute__((aligned(PAGE_SIZE)));

/*
 *
 */
struct CapabilitySlab {
	List FreeSlabs;
	List UsedSlabs;
	List FullSlabs;
};

/*
 *
 */
struct CapabilitySpace {
	CapabilitySlab Slabs;
	CapabilityTreeNode *CapabilityTree;
	CapabilityTreeNode *CPUCapabilityTree;
	CapabilityTreeNode *IOCapabilityTree;
};

struct ContainerIdentifier {
	u8 Secret[SECP256k1_SHARED_SECRET_SIZE];
	u8 IV[AES_BLOCKLEN];
}__attribute__((aligned(0x10)));

struct VirtualCPU {
	u8 ID;

#if defined(__x86_64__)
	x86::VMData *Data;
#endif
}__attribute__((aligned(PAGE_SIZE)));

struct SchedulerContext {
	uptr SP;
	uptr BP;
	uptr IP;
	
#if defined(__x86_64__)
	x86::GeneralRegisters Registers;
	u64 RFLAGS;
#endif
	
	bool InKernel;

	usize Budget;
}__attribute__((packed));

struct ContainerBindings {
	void (*ExceptionHandler)(usize excp, usize errinfo1, usize errinfo2);
	void (*InterruptHandler)();
	void (*SyscallHandler)();
}__attribute__((packed));

struct ContainerInfo {
	uptr InitrdAddress;
	usize InitrdSize;

	union {
#if defined(__x86_64__)
	struct {
		uptr RSDPCapability;
		usize RSDPOffset;
	} x86_64;
#endif
	};

	uptr DTB;
}__attribute__((packed));

struct Container : public ListHead {
	ContainerIdentifier Identifier;
	ContainerBindings Bindings;

	CapabilitySpace CSpace;
	VirtualSpace MemorySpace;
	SchedulerContext Context;

	VirtualCPU *VCPU;

	uptr LowestKernelAddress;
	uptr HighestKernelAddress;
}__attribute__((aligned(0x10)));



