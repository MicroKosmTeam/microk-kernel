#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

#if defined(__x86_64__)
#include <arch/x86/object.hpp>
#endif

/* Structure that is the parent of all objects
 * that will belong in a linked ListHead
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

/* All the available object types that can be
 * mapped to a capability.
 *
 * 00h - NULL_CAPABILITY:
 * 01h - UNTYPED:
 * 02h - FRAMES:
 * 03h - CSPACE:
 * 04h - CNODE:
 * 05h - DOMAIN:
 * 06h - SCHEDULER:
 * 07h - TASK_CONTROL_BLOCK:
 * 08h - SCHEDULER_CONTEXT:
 * 09h - PAGING_STRUCTURE:
 * 0Ah - OBJECT_TYPE_COUNT:
 *  Constant that keeps count of the total amount
 *  of allowed object types.
 * FFh - RESERVED_SLOT:
 *  Statically assigned slots that haven't yet been filled.
 */
enum ObjectType {
	NULL_CAPABILITY = 0,
	UNTYPED,
	FRAMES,
	CSPACE,
	CNODE,
	DOMAIN,
	SCHEDULER,
	TASK_CONTROL_BLOCK,
	SCHEDULER_CONTEXT,
	PAGING_STRUCTURE,
	OBJECT_TYPE_COUNT,
	RESERVED_SLOT = 0xFF
};

/* The rights that can be given to a capability
 *
 *      0 - NONE:
 * 1 << 0 - READ:
 * 1 << 1 - WRITE:
 * 1 << 2 - EXECUTE:
 * 1 << 3 - GRANT:
 * 1 << 4 - REVOKE:
 * 1 << 5 - MINT:
 * 1 << 6 - RETYPE:
 * 1 << 7 - MODIFY:
 *  Allows the object to be 
 */
enum CapabilityRights {
	NONE = 0,
	READ = 1 << 0,
	WRITE = 1 << 1,
	EXECUTE = 1 << 2,
	GRANT = 1 << 3,
	REVOKE = 1 << 4,
	MINT = 1 << 5,
	RETYPE = 1 << 6,
};

/*
 *
 */
struct Capability {
	u8 Type;
	uptr Object;
	usize Size;
	u32 AccessRights;
}__attribute__((packed, aligned(0x8)));

enum RootCNodeSlots {
	NULL_SLOT = 0,
	CSPACE_SLOT,
	ROOT_CNODE_SLOT,
	TASK_CONTROL_BLOCK_SLOT,
	MEMORY_MAP_CNODE_SLOT,
	FIRST_FREE_SLOT,
};

/*
 *
 */
struct CapabilityNode : public ListHead {
	Capability Slots[PAGE_SIZE / sizeof(Capability)];
}__attribute__((aligned(PAGE_SIZE)));

/*
 *
 */
struct CapabilitySpace {
	List CapabilityNodeList;
};

/*
 *
 */
typedef uptr VirtualSpace;

struct ThreadControlBlock;
struct SchedulerContext;
struct Scheduler;

enum ThreadStatus {
	IDLING = 0xFF, /* Special, used for Idle task */
	RUNNING = 0,
	WAITING,
	BLOCKED,
};

struct Domain {
	Scheduler *DomainScheduler;
};

/* The scheduler itself, one exists for each domain
 * Threads can't therefore be passed from one domain to
 * another. Because of the absence of the knowledge of processes,
 * however, that implies that threads can be created on another
 * domain but still have the same CSpace and VirtualSpace
 */
struct Scheduler {
	Domain *Parent;
	ThreadControlBlock *Running;

	List Waiting[SCHEDULER_PRIORITIES];
	List Blocked[SCHEDULER_PRIORITIES];
}__attribute__((aligned(PAGE_SIZE)));

struct ThreadControlBlock : public ListHead {
	ThreadStatus Status;
	usize TaskID;
	u8 Priority;

	VirtualSpace MemorySpace;
	CapabilitySpace *CSpace;
	SchedulerContext *Context;
};

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


