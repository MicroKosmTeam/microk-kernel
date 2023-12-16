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

void AddElementToList(ListHead *element, List *list);
void AddElementToList(ListHead *element, ListHead *prev, List *list);

void MoveElementToList(ListHead *element, List *origin, List *destination);

ListHead *PopListHead(List *list);
inline __attribute__((always_inline))
void PushListTail(ListHead *element, List *list) {
	AddElementToList(element, list);
}
