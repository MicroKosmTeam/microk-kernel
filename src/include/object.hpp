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
	u8 Type;
	u8 ExtraAttributes;
	uptr Object;
	u16 AccessRights;
	u16 Children;
	Capability *Parent;
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

	/* From here on out, architecture specific stuff goes here */
#if defined(__x86_64__)
	__attribute__((aligned(0x100))) x86::GDT Gdt;
	__attribute__((aligned(0x100))) x86::GDTPointer GdtPointer;
	__attribute__((aligned(0x100))) x86::TSS Tss;
//	__attribute__((aligned(0x100))) x86::APIC Apic;
#endif
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
