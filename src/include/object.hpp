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
};

struct CapabilityRights {
	u8 Read : 1;
	u8 Write : 1;
	u8 Grant : 1;
	u8 Revoke : 1;
	u8 Retype : 1;
}__attribute__((packed));

struct Capability {
	ObjectType Type;
	uptr Object;
	CapabilityRights AccessRights;
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
