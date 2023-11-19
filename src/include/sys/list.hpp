#pragma once
#include <cstdint.hpp>

struct ListHead {
	ListHead *Next, *Previous;
};

struct List {
	ListHead *Head;
	ListHead *Tail;
};

inline __attribute__((always_inline))
ListHead *GetListHead(List *list) {
	return list->Head;
}

void MoveElementToList(ListHead *element, List *origin, List *destination);
void AddElementToList(ListHead *element, List *list);
