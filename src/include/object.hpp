#pragma once
#include <cstdint.hpp>

struct ListHead {
	ListHead *Next, *Previous;
};

struct List {
	ListHead *Head;
	ListHead *Tail;
};

enum ObjectType {
	NULL_CAPABILITY = 0,
	UNTYPED_MEMORY,
	FRAMES,
	TCB,
};

enum CapabilityRights {
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

	VirtualSpace *MemorySpace;
	SchedulerContext *Context;
	CapabilitySpace *CSpace;
};

struct SchedulerContext {
	usize Budget;
};
