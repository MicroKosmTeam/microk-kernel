#pragma once
#include <stddef.h>
#include <mm/memory.hpp>

template <typename Type> class Vector {
public:
	Vector() {
		Init();
	}

	~Vector() {
		delete[] DataArray;
	}

	void Init() {
		CurrentElement = 0;
		Capacity = 1;
		DataArray = new Type[1];
		if (DataArray == NULL) Capacity = 0;
	}

	bool Push(Type data) {
		if (CurrentElement == Capacity) {
			Type *TemporaryArray = new Type[2 * Capacity];
			if (TemporaryArray == NULL) return false;

			for (size_t i = 0; i < CurrentElement; i++) {
				TemporaryArray[i] = DataArray[i];
			}

			delete[] DataArray;

			Capacity *= 2;

			DataArray = TemporaryArray;
		}

		DataArray[CurrentElement] = data;
		CurrentElement++;

		return true;
	}

	bool Push(Type data, int index) {
		if (index >= Capacity) {
			size_t ratio = 1;

			while (index >= Capacity * ratio) ratio *= 2;

			Type *TemporaryArray = new Type[Capacity * ratio];
			if (TemporaryArray == NULL) return false;

			for (size_t i = 0; i < CurrentElement; i++) {
				TemporaryArray[i] = DataArray[i];
			}

			delete[] DataArray;

			Capacity *= ratio;

			DataArray = TemporaryArray;
		}
	
		DataArray[index] = data;

		if (index >= CurrentElement) CurrentElement += index - CurrentElement + 1;

		return true;
	}

	Type Get(int index) {
		if (index < CurrentElement) {
			return DataArray[index];
		} else {
			return NULL;
		}
	}

	Type Pop() {
		if (CurrentElement == 0) return NULL;

		return DataArray[CurrentElement--];

	}

	size_t GetSize() {
		return CurrentElement;

	}

	size_t GetMemorySize() {
		return Capacity * sizeof(Type);

	}

	size_t GetCapacity() {
		return Capacity;

	}
private:
	Type *DataArray;
	size_t Capacity;
	size_t CurrentElement;

};
