#include <array.h>
#include <utils.h>

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef _DEBUG
struct {
	bool constructors;
	bool destructors;
	bool resize;
	bool misc;
} DebugMode;
#endif

#ifdef _DEBUG
void Array_SetDebugMode(uint8_t value) {
	DebugMode.constructors = value & ARRAY_DEBUG_CONSTRUCTORS;
	DebugMode.destructors = value & ARRAY_DEBUG_DESTRUCTORS;
	DebugMode.resize = value & ARRAY_DEBUG_RESIZE;
	DebugMode.misc = value & ARRAY_DEBUG_MISC;
}
uint8_t Array_GetDebugMode() {
	uint8_t value = 0;
	if (DebugMode.constructors) value |= ARRAY_DEBUG_CONSTRUCTORS;
	if (DebugMode.destructors) value |= ARRAY_DEBUG_DESTRUCTORS;
	if (DebugMode.resize) value |= ARRAY_DEBUG_RESIZE;
	if (DebugMode.misc) value |= ARRAY_DEBUG_MISC;
	return value;
}
#endif

struct Array {
	char* data;
	size_t reservedSize;
	size_t length;
};

Array* Array_Create(size_t size) {
	Array* array = (Array*)calloc(1, sizeof(Array));
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Failed to allocate memory for Array");
#endif
		return NULL;
	}

	array->reservedSize = size + 1;
	array->length = 0;
	array->data = (char*)calloc(array->reservedSize, sizeof(char));
	return array;
}

Array* Array_Copy(const Array* const array) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Array is NULL");
#endif
		return NULL;
	}
	Array* copy = Array_Create(array->length);
	if (!copy) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Failed to create copy of Array");
#endif
		return NULL;
	}

	copy->length = array->length;
	memcpy(copy->data, array->data, array->length);
	return copy;
}

Array* Array_CreateFromString(const char* const string) {
	if (!string) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("String data is NULL");
#endif
		return false;
	}

	size_t size = strlen(string);
	Array* arr = Array_Create(size);
	if (!arr) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Failed to create Array from string");
#endif
		return NULL;
	}

	if (!Array_EmplaceBackSize(arr, string, size)) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Cannot emplace back string");
#endif
	}
	return arr;
}

bool Array_Reserve(Array* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
#endif
		return false;
	}
	if (size == 0) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Size is 0");
#endif
		return false;
	}

	if (array->reservedSize > size)
		return true;

	return Array_Resize(array, size);
}

bool Array_Resize(Array* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
#endif
		return false;
	}
	if (!array->data) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Array data is NULL");
#endif
		return false;
	}

	if (array->reservedSize == size)
		return true;
    
	size_t newReservedSize = size + 1;
	void* temp = realloc(array->data, newReservedSize * sizeof(char));
	if (!temp) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Failed to allocate memory for Array");
#endif
		return false;
	}

	array->data = temp;
	array->reservedSize = newReservedSize;
	return true;
}

bool Array_PushBack(Array* array, char value) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
#endif
		return false;
	}

	if (!Array_Reserve(array, nearestMultipleOf(array->length + 1, 8))) {
		return false;
	}

	array->data[array->length] = value;
	array->data[++array->length] = '\0';
	return true;
}

bool Array_EmplaceBack(Array* array, const char* const value) {
	if (!array || !value) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
#endif
		return false;
	}

	size_t valueLength = strlen(value);
	size_t newSize = array->length + valueLength + 1;
	if (!Array_Reserve(array, nearestMultipleOf(newSize, 8))) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Cannot reserve space");
#endif
		return false;
	}

	return Array_EmplaceBackSize(array, value, valueLength);
}
bool Array_EmplaceBackSize(Array* array, const char* const value, size_t size) {
	if (!array || !value)
		return false;

	size_t newSize = array->length + size + 1;
	if (!Array_Reserve(array, nearestMultipleOf(newSize, 8))) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Cannot reserve space");
#endif
		return false;
	}

	memcpy(array->data + array->length, value, size);
	array->length += size;
	return true;
}

char Array_At(const Array* const array, size_t index) {
	if (!array || index >= array->length)
		return 0;
	return array->data[index];
}
char* Array_GetData(const Array* const array) {
	if (!array)
		return NULL;
	return array->data;
}

size_t Array_GetSize(const Array* const array) {
	if (!array)
		return -1;
	return array->length;
}

size_t Array_GetReservedSize(const Array* const array) {
	if (!array)
		return -1;
	return array->reservedSize;
}

void Array_Destroy(Array* array) {
	if (!array) {
#ifdef _DEBUG
		if(DebugMode.destructors)
			LOG_ERR("Array is NULL");
#endif
		return;
	}
	if (!array->data) {
#ifdef _DEBUG
		if (DebugMode.destructors)
			LOG_ERR("Array data is NULL");
#endif
		return;
	}

	free(array->data);
	array->data = NULL;

	free(array);
	array = NULL;
}


struct Vector {
	Array** data;
	size_t reservedSize;
	size_t length;
};

Vector* Vector_Create(size_t size) {
	Vector* array = (Vector*)calloc(1, sizeof(Vector));
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.constructors)
			LOG_ERR("Failed to allocate memory for Vector");
#endif
		return NULL;
	}

	array->reservedSize = size + 1;
	array->length = 0;
	array->data = (Array**)calloc(size + 1, sizeof(Array*));
	if (!array->data)
		return NULL;
	return array;
}

bool Vector_Reserve(Vector* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
#endif
		return false;
	}

	if (array->reservedSize > size)
		return true;

	return Vector_Resize(array, size);
}
bool Vector_Resize(Vector* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
#endif
		return false;
	}

	if (array->reservedSize == size)
		return true;

	size_t newReservedSize = size + 1;
	Array** temp = realloc(array->data, newReservedSize * sizeof(Array*));
	if (!temp) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Failed to allocate memory for Array");
#endif
		return false;
	}

	array->data = temp;
	array->reservedSize = newReservedSize;
	return true;
}

bool Vector_PushBack(Vector* array, const char* const value) {
	Array* arr = Array_CreateFromString(value);
	return Vector_PushBackArray(array, arr);
}
bool Vector_PushBackArray(Vector* array, const Array* const value) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
#endif
		return false;
	}
	if (!value) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Value is NULL");
#endif
		return false;
	}

	if (!Vector_Reserve(array, nearestMultipleOf(array->length + 1, 8))) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Cannot reserve the data");
#endif
		return false;
	}

	Array* copy = Array_Copy(value);
	if (!copy) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Cannot copy the array");
#endif
		return false;
	}

	array->data[array->length] = copy;
	array->length++;
	return true;
}

const Array* const Vector_At(const Vector* const array, size_t index) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.misc)
			LOG_ERR("Vector is NULL");
#endif
		return NULL;
	}

	if (index >= array->length) {
#ifdef _DEBUG
		if (DebugMode.misc)
			LOG_ERR("Vector index out of bounds");
#endif
		return NULL;
	}

	return array->data[index];
}

size_t Vector_GetSize(const Vector* const array) {
	if (!array)
		return -1;
	return array->length;
}
size_t Vector_GetReservedSize(const Vector* const array) {
	if (!array)
		return -1;
	return array->reservedSize;
}

void Vector_Destroy(Vector* array) {
	if (!array) {
#ifdef _DEBUG
		if (DebugMode.destructors)
			LOG_ERR("Vector is NULL");
#endif
		return;
	}

	for (size_t i = 0; i < array->length; i++) {
#ifdef _DEBUG
		if (DebugMode.destructors)
			LOG("Deleting element %llu in 0x%08" PRIX64 "...", i, (uintptr_t)array);
#endif
		Array_Destroy(array->data[i]);
		array->data[i] = NULL;
	}

	free(array->data);
	free(array);
	array = NULL;
}
