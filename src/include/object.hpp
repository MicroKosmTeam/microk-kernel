#pragma once
#include <cdefs.h>

#if defined(__x86_64__)
#include <arch/x86/object.hpp>
#elif defined(__aarch64__)
#include <arch/aarch64/object.hpp>

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
	uptr Object;

	u16 AccessRights;
	u16 AccessRightsMask;

	// TODO: overhaul parent/child relationship
	//Capability *Parent;
}__attribute__((packed, aligned(0x8)));

/*
 *
 */
struct UntypedHeader {
	uptr Address;
	usize Length;
	u32 Flags;
}__attribute__((packed));

/*
 *  CAPABILITY MEMORY STRUCTURE:
 *   Space contains slabs for each kind of capability
 *   Slabs contain various nodes where the capabilities are stored.
 *   Whenever a program requests a capability, the kernel grants it from the
 *   appropriate slab. If there are no available specific kinds of objects,
 *   the program must retype an untyped object. If the the nodes are all full, 
 *   the program must expand them by retyping an untyped object.
 *   Untyped objects are not shared between programs, while others may be,
 *   for examples TCBs.
 */


/*
 *
 */

struct SlabHead : public ListHead {
	u16 FreeElements;
};

#define CAPABILITIES_PER_NODE ((PAGE_SIZE - sizeof(SlabHead)) / sizeof(Capability))

/*
 *
 */
struct CapabilityNode : public SlabHead {
	Capability Slots[CAPABILITIES_PER_NODE];
};

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
	CapabilitySlab Slabs[OBJECT_TYPE_COUNT];
};

/*
 *
 */
struct TaskControlBlock;
struct SchedulerContext;
struct Scheduler;


/*
 *
 */
enum EndpointStatus {
	EP_EMPTY = 1,
	EP_WAITING,
	EP_SEND,
	EP_RECV,
};

/*
 *
 */
struct Endpoint {
	TaskControlBlock *Thread;
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
	TaskControlBlock *Running;

	List Waiting[SCHEDULER_PRIORITIES];
	List Blocked[SCHEDULER_PRIORITIES];
}__attribute__((aligned(PAGE_SIZE)));

struct TaskControlBlock : public ListHead {
	Scheduler *Parent;

	/* Used to queue up the thread in endpoints and such.
	 * Also, because a TCB can only be waiting in one EP
	 * (thanks to the blocking behavior) we only need one
	 * list head per TaskControlBlock.
	 */
	ListHead ActionQueue;

	ThreadStatus Status;
	usize TaskID;
	u8 Priority;

	u8 *VirtualRegisters;

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
