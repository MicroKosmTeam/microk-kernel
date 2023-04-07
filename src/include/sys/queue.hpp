#pragma once
#include <stddef.h>
#include <mm/memory.hpp>

template <typename Type> class Queue {
public:
	Queue() {
		TotalElements = 0;
		First = new QueueNode;
		Last = new QueueNode;
		if (First == NULL || Last == NULL) return;
		
		First->Previous = NULL;
		First->Next = Last;
		Last->Previous = First;
		Last->Next = NULL;

	}

	~Queue() {
		QueueNode *current = Last;
		while(true) {
			QueueNode *next = current->Previous;
			delete current;
			current = next;

			if (current == NULL) break;
		}
	}

	bool Push(Type data) {
		QueueNode *oldPrevious = Last->Previous;

		Last->Previous = new QueueNode;
		if (Last->Previous == NULL) return false;

		Last->Previous->Previous = oldPrevious;
		Last->Previous->Previous->Next = Last->Previous;
		Last->Previous->Data = data;
		Last->Previous->Next = Last;

		TotalElements++;
		return true;
	}

	bool Pop(Type *data) {
		if (TotalElements == 0) return false;

		QueueNode *newFirst = First->Next->Next;
		if(newFirst == NULL) return false;

		*data = First->Next->Data;
		delete First->Next;
		First->Next = newFirst;

		TotalElements--;

		return true; 
	}

	size_t GetElements() {
		return TotalElements;

	}
private:
	struct QueueNode {
		QueueNode *Previous;
		Type Data;
		QueueNode *Next;
	};

	QueueNode *First;
	QueueNode *Last;
	size_t TotalElements;

};

