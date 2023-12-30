#pragma once
#include <cstdint.hpp>

#if defined(__x86_64__)
#include <arch/x86/object.hpp>
#endif

struct ListHead {
	ListHead *Next, *Previous;
};

struct List {
	ListHead *Head;
	ListHead *Tail;
};

enum ObjectType {
	NULL_CAPABILITY = 0,
	UNTYPED,
	CSPACE,
	CNODE,
	FRAMES,
	TASK_CONTROL_BLOCK,
	SCHEDULER_CONTEXT,
};

enum CapabilityRights {
	NONE = 0,
	READ = 1 << 0,
	WRITE = 1 << 1,
	GRANT = 1 << 2,
	REVOKE = 1 << 3,
	RETYPE = 1 << 4,
};

struct Capability {
	ObjectType Type;
	uptr Object;
	usize Size;
	u32 AccessRights;
};

struct CapabilityNode : public ListHead {
	Capability Slots[];
};

struct CapabilitySpace {
	List CapabilityNodeList;
};

struct VirtualSpace {
	uptr VirtualHierarchyTop;
};

struct ThreadControlBlock;
struct SchedulerContext;

struct ThreadControlBlock : public ListHead {
	usize TaskID;
	u8 Priority;

	VirtualSpace *MemorySpace;
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
