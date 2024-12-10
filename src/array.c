#include <array.h>
#include <utils.h>

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

struct {
	bool constructors;
	bool destructors;
	bool resize;
	bool push;
	bool lookup;
	bool misc;
} DebugMode;

void Array_SetDebugMode(uint8_t value) {
	DebugMode.constructors = value & ARRAY_DEBUG_CONSTRUCTORS;
	DebugMode.destructors = value & ARRAY_DEBUG_DESTRUCTORS;
	DebugMode.resize = value & ARRAY_DEBUG_RESIZE;
	DebugMode.push = value & ARRAY_DEBUG_PUSH;
	DebugMode.lookup = value & ARRAY_DEBUG_LOOKUP;
	DebugMode.misc = value & ARRAY_DEBUG_MISC;
}
uint8_t Array_GetDebugMode() {
	uint8_t value = 0;
	if (DebugMode.constructors) value |= ARRAY_DEBUG_CONSTRUCTORS;
	if (DebugMode.destructors) value |= ARRAY_DEBUG_DESTRUCTORS;
	if (DebugMode.resize) value |= ARRAY_DEBUG_RESIZE;
	if (DebugMode.push) value |= ARRAY_DEBUG_PUSH;
	if (DebugMode.lookup) value |= ARRAY_DEBUG_LOOKUP;
	if (DebugMode.misc) value |= ARRAY_DEBUG_MISC;
	return value;
}

struct Array {
	char* data;
	size_t reservedSize;
	size_t length;
};

Array* Array_Create(size_t size) {
	Array* array = (Array*)calloc(1, sizeof(Array));
	assert(array && "Cannot allocate array");
	if (!array) {
		if (DebugMode.constructors)
			LOG_ERR("Failed to allocate memory for Array");
		return NULL;
	}

	array->reservedSize = size + 1;
	array->length = 0;
	array->data = (char*)calloc(array->reservedSize, sizeof(char));
	return array;
}

Array* Array_Copy(const Array* const array) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.constructors)
			LOG_ERR("Array is NULL");
		return NULL;
	}
	Array* copy = Array_Create(array->length);
	if (!copy) {
		if (DebugMode.constructors)
			LOG_ERR("Failed to create copy of Array");
		return NULL;
	}

	copy->length = array->length;
	memcpy(copy->data, array->data, array->length);
	return copy;
}

Array* Array_CreateFromString(const char* const string) {
	assert(string && "String is NULL");
	if (!string) {
		if (DebugMode.constructors)
			LOG_ERR("String data is NULL");
		return false;
	}

	size_t size = strlen(string);
	Array* arr = Array_Create(size);
	if (!arr) {
		if (DebugMode.constructors)
			LOG_ERR("Failed to create Array from string");
		return NULL;
	}

	if (!Array_EmplaceBackSize(arr, string, size)) {
		if (DebugMode.constructors)
			LOG_ERR("Cannot emplace back string");
	}
	return arr;
}

bool Array_Reserve(Array* array, size_t size) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
		return false;
	}
	if (size == 0) {
		if (DebugMode.resize)
			LOG_ERR("Size is 0");
		return false;
	}

	if (array->reservedSize > size)
		return true;

	return Array_Resize(array, size);
}

bool Array_Resize(Array* array, size_t size) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.resize)
			LOG_ERR("Array is NULL");
		return false;
	}
	if (!array->data) {
		if (DebugMode.resize)
			LOG_ERR("Array data is NULL");
		return false;
	}

	if (array->reservedSize == size)
		return true;
    
	size_t newReservedSize = size + 1;
	void* temp = realloc(array->data, newReservedSize * sizeof(char));
	if (!temp) {
		if (DebugMode.resize)
			LOG_ERR("Failed to allocate memory for Array");
		return false;
	}

	array->data = temp;
	array->reservedSize = newReservedSize;
	return true;
}

bool Array_PushBack(Array* array, char value) {
	if (!array) {
		if (DebugMode.push)
			LOG_ERR("Array is NULL");
		return false;
	}

	if (!Array_Reserve(array, nearestMultipleOf(array->length + 1, 8))) {
		if (DebugMode.push)
			LOG_ERR("Cannot reserve space for the element");
		return false;
	}

	array->data[array->length] = value;
	array->data[++array->length] = '\0';
	return true;
}

bool Array_EmplaceBack(Array* array, const char* const string) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.push)
			LOG_ERR("Array is NULL");
		return false;
	}
	assert(string && "String is NULL");
	if (!string) {
		if (DebugMode.push)
			LOG_ERR("String is NULL");
		return false;
	}

	size_t valueLength = strlen(string);
	size_t newSize = array->length + valueLength + 1;
	if (!Array_Reserve(array, nearestMultipleOf(newSize, 8))) {
		if (DebugMode.push)
			LOG_ERR("Cannot reserve space for the string");
		return false;
	}

	return Array_EmplaceBackSize(array, string, valueLength);
}
bool Array_EmplaceBackSize(Array* array, const char* const string, size_t size) {
	assert(array && "Array is NULL");
	if (!array)
		return false;

	assert(array && "String is NULL");
	if (!string)
		return false;

	size_t newSize = array->length + size + 1;
	if (!Array_Reserve(array, nearestMultipleOf(newSize, 8))) {
		if (DebugMode.push)
			LOG_ERR("Cannot reserve space for the string");
		return false;
	}

	memcpy(array->data + array->length, string, size);
	array->length += size;
	return true;
}

char Array_At(const Array* const array, size_t index) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.lookup)
			LOG_ERR("Array is NULL");
		return 0;
	}
	assert(index < array->length && "Index out of bounds");
	if (index >= array->length) {
		if (DebugMode.lookup)
			LOG_ERR("Index out of bounds");
		return 0;
	}
	return array->data[index];
}
char* Array_GetData(const Array* const array) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.lookup)
			LOG_ERR("Array is NULL");
		return NULL;
	}
	return array->data;
}

size_t Array_GetSize(const Array* const array) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.lookup)
			LOG_ERR("Array is NULL");
		return -1;
	}
	return array->length;
}

size_t Array_GetReservedSize(const Array* const array) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.lookup)
			LOG_ERR("Array is NULL");
		return -1;
	}
	return array->reservedSize;
}

void Array_Destroy(Array* array) {
	assert(array && "Array is NULL");
	if (!array) {
		if (DebugMode.destructors)
			LOG_ERR("Array is NULL");
		return;
	}
	assert(array->data && "Array is already destroyed");
	if (!array->data) {
		if (DebugMode.destructors)
			LOG_ERR("Array data is NULL");
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
		if (DebugMode.constructors)
			LOG_ERR("Failed to allocate memory for Vector");
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
		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
		return false;
	}

	if (array->reservedSize > size)
		return true;

	return Vector_Resize(array, size);
}
bool Vector_Resize(Vector* array, size_t size) {
	if (!array) {

		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
		return false;
	}

	if (array->reservedSize == size)
		return true;

	size_t newReservedSize = size + 1;
	Array** temp = realloc(array->data, newReservedSize * sizeof(Array*));
	if (!temp) {

		if (DebugMode.resize)
			LOG_ERR("Failed to allocate memory for Array");
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

		if (DebugMode.resize)
			LOG_ERR("Vector is NULL");
		return false;
	}
	if (!value) {

		if (DebugMode.resize)
			LOG_ERR("Value is NULL");
		return false;
	}

	if (!Vector_Reserve(array, nearestMultipleOf(array->length + 1, 8))) {

		if (DebugMode.resize)
			LOG_ERR("Cannot reserve the data");
		return false;
	}

	Array* copy = Array_Copy(value);
	if (!copy) {

		if (DebugMode.resize)
			LOG_ERR("Cannot copy the array");
		return false;
	}

	array->data[array->length] = copy;
	array->length++;
	return true;
}

const Array* const Vector_At(const Vector* const array, size_t index) {
	if (!array) {

		if (DebugMode.misc)
			LOG_ERR("Vector is NULL");
		return NULL;
	}

	if (index >= array->length) {

		if (DebugMode.misc)
			LOG_ERR("Vector index out of bounds");
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

		if (DebugMode.destructors)
			LOG_ERR("Vector is NULL");
		return;
	}

	for (size_t i = 0; i < array->length; i++) {

		if (DebugMode.destructors)
			LOG("Deleting element %llu in 0x%08" PRIX64 "...", i, (uintptr_t)array);
		Array_Destroy(array->data[i]);
		array->data[i] = NULL;
	}

	free(array->data);
	free(array);
	array = NULL;
}
