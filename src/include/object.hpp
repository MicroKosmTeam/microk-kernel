#pragma once
#include "cdefs.h"
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
 * 0 - NULL_CAPABILITY:
 * 1 - UNTYPED:
 * 2 - FRAMES:
 * 3 - CSPACE:
 * 4 - CNODE:
 * 5 - DOMAIN:
 * 6 - SCHEDULER:
 * 7 - TASK_CONTROL_BLOCK:
 * 8 - SCHEDULER_CONTEXT:
 * 9 - OBJECT_TYPE_COUNT:
 *  Constant that keeps count of the total amount
 *  of allowed object types.
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
	OBJECT_TYPE_COUNT,
};

/* The rights that can be given to a capability
 *
 * 0 << 0 - NONE:
 *  The capability exists only to make sure that resource
 *  can't be touched by userspace in any way. It renders
 *  the object unusable and its status is unrevokable.
 *
 * 1 << 0 - READ:
 *  Allows the object to be mapped in virtual memory and be
 *  accessed. Any illegal write will cause a page fault.
 *
 * 1 << 1 - WRITE:
 *  Allows the object to be mapped in virtual memory and be
 *  written to.
 *  Note that it can be used without READ permissions,
 *  but it won't make any sense on some architectures, as the page
 *  will be marked as writable but not accessible, causing page faults.
 *
 * 1 << 2 - EXECUTE:
 *  Allows the object to be mapped in virtual memory and be
 *  executed. If the architecture has hardware protection, this will
 *  make a difference, otherwise it will just be used as information
 *  that the pointed object is a piece of executable code and should be
 *  treated accordingly.
 *  Note that it can be used without READ permissions,
 *  but it won't make any sense on some architectures, as the page
 *  will be marked as executable but not accessible, causing page faults.
 *
 * 1 << 3 - GRANT:
 * 1 << 4 - REVOKE
 * 1 << 5 - MINT
 * 1 << 6 - RETYPE:
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
	ObjectType Type;
	uptr Object;
	usize Size;
	u32 AccessRights;
};

/*
 *
 */
struct CapabilityNode : public ListHead {
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
};

struct ThreadControlBlock : public ListHead {
	ThreadStatus Status;
	usize TaskID;
	u8 Priority;

	VirtualSpace MemorySpace;
	SchedulerContext *Context;
	CapabilitySpace *CSpace;
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


