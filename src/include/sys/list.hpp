#pragma once
#include <stddef.h>
#include <mm/memory.hpp>
/*
template <typename Type> class List {
public:
	List() {
		Init();
	}

	~List() {
		delete[] DataArray;
	}

	bool PushLast(Type data) {
		Node *newNode = new Node;
		newNode->Data = data;

		newNode->Previous = Last;
		newNode->Next = NULL;
		
		if (Last) Last->Next = newNode;

		Last = newNode;

		++Length;

		return true;
	}
	
	bool PushFirst(Type data) {
		Node *newNode = new Node;
		newNode->Data = data;

		newNode->Previous = NULL;
		newNode->Next = First;
		
		if (First) First->Previous = newNode;

		First = newNode;
		
		++Length;

		return true;
	}

	Type PopLast() {
		if(Last == NULL) return NULL;

		Type value = Last->Data;

		if(Last->Previous) Last->Previous->Next = NULL;
		Node *newLast = Last->Previous;

		delete Last;
		Last = newLast;
			
		--Length;

		return value;
	}

	Type PopFirst() {
		Type value = First->Data;

		if(First->Next) First->Next->Previous = NULL;
		Node *newFirst = First->Next;

		delete First;
		First = newFirst;
	
		--Length;

		return value;
	}

	size_t GetMemorySize() {
		return Length * sizeof(Node);

	}

	size_t GetLength() {
		return Length;

	}
private:
	struct Node {
		Type Data;
		Node *Next;
		Node *Previous;
	};

	Node *First;
	Node *Last;

	Node *LastAccessed;

	size_t Length;

	void Init() {
		First = Last = LastAccessed = NewNode;
		First->Next = Last;
		Last->Previous= First;

		Length = 0;
	}
};*/
