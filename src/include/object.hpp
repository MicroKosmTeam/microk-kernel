#pragma once
#include <cdefs.h>

#if defined(__x86_64__)
#include <arch/x86/object.hpp>
#elif defined(__aarch64__)
#include <arch/aarch64/object.hpp>

#endif

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
 *       Nothing can be done with the capability.
 *
 * 1 << 0 - ACCESS:
 *       The capability can used by getting its pointer.
 *
 * 1 << 1 - SEE:
 *       The capability struct can be visualized by userspace.
 *
 * 1 << 2 - RETYPE:
 *       The object can be retyped to another kind of object.
 *
 * 1 << 3 - GRANT:
 *       The ownership of the capability can be given to some other CSpace
 *       (not reversible).
 *
 * 1 << 4 - MINT:
 * 	 Can create a new capability can be forged with the same or fewer
 * 	 priviledges and then granted to someone else.
 *
 * 1 << 5 - REVOKE:
 *       The capability can be revoked and it will become UNTYPED.
 *
 *  -- FOR FRAMES ONLY --
 * 1 << 6 - READ:
 *       The frame can be mapped in the virtual space.
 *
 * 1 << 7 - WRITE:
 *       The frame can be mapped in the virtual space and written to.
 *
 * 1 << 8 - EXECUTE:
 *       The frame can be mapped in the virtual space and have NX
 *       protection disabled on it.
 */
enum CapabilityRights {
	NONE = 0,
	ACCESS = 1 << 0,
	SEE = 1 << 1,
	RETYPE = 1 << 2,
	GRANT = 1 << 3,
	MINT = 1 << 4,
	REVOKE = 1 << 5,
	READ = 1 << 6,
	WRITE = 1 << 7,
	EXECUTE = 1 << 8,
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
	VIRTUAL_SPACE_ROOT_SLOT,
	MEMORY_MAP_CNODE_SLOT,
	INIT_INFO_FRAME_SLOT,
	SLOT_COUNT,
};

/*
 *
 */
struct CapabilityNode : public ListHead {
	u16 SizeBits;
	Capability Slots[];
};

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


/*
 *
 */
enum EndpointStatus {
	EP_IDLE = 1,
	EP_SEND,
	EP_RECV,
};

/*
 *
 */
struct Endpoint {
	ThreadControlBlock *Thread;
	EndpointStatus Status;

	List ThreadQueue;
};

enum NotificationStatus {
	NT_IDLE = 1,
};

struct Notification {
	NotificationStatus Status;
};

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
	Scheduler *Parent;

	/* Used to queue up the thread in endpoints and such.
	 * Also, because a TCB can only be waiting in one EP
	 * (thanks to the blocking behavior) we only need one
	 * list head per ThreadControlBlock.
	 */
	ListHead ActionQueue;

	ThreadStatus Status;
	usize TaskID;
	u8 Priority;

	u32 VirtualRegisters[VIRTUAL_REGISTERS_SIZE];

	VirtualSpace MemorySpace;
	CapabilityNode *RootCNode;
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

struct IInfo {
}__attribute__((packed));

