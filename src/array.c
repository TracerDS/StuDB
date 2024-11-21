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
} DebugMode;
#endif

#ifdef _DEBUG
void Array_SetDebugMode(uint8_t value) {
	DebugMode.constructors = value & 0x1;
	DebugMode.constructors = value & 0x2;
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
	if (!array)
		return false;

	if (array->reservedSize <= size)
		return true;

	return Array_Resize(array, size);
}

bool Array_Resize(Array* array, size_t size) {
	if (!array || !array->data)
		return false;

	if (array->reservedSize == size)
		return true;

	size_t newReservedSize = size + 1;

	char* newData = (char*)calloc(newReservedSize, sizeof(char));
	if (!newData) {
#ifdef _DEBUG
		if (DebugMode.resize)
			LOG_ERR("Failed to allocate memory for Array");
#endif
		return false;
	}

	// Copy existing data to the new buffer
	size_t bytesToCopy = array->length;
	if (size < array->length) {
		bytesToCopy = size;
		array->length = size;
	}
	memcpy(newData, array->data, bytesToCopy);

	if (bytesToCopy < newReservedSize)
		newData[bytesToCopy] = '\0';

	// Free the old memory and update the array
	free(array->data);
	array->data = newData;
	array->reservedSize = newReservedSize;

	return true;
}

bool Array_PushBack(Array* array, char value) {
	if (!array)
		return false;

	if (!Array_Reserve(array, nearestMultipleOf(array->length + 1, 8))) {
		return false;
	}

	array->data[array->length] = value;
	array->data[++array->length] = '\0';
	return true;
}

bool Array_EmplaceBack(Array* array, const char* const value) {
	if (!array || !value)
		return false;

	size_t valueLength = strlen(value);
	size_t newSize = array->length + valueLength + 1;
	if (newSize >= array->reservedSize) {
		newSize = nearestMultipleOf(newSize, 8);
		if (!Array_Resize(array, newSize))
			return false;
	}

	memcpy(array->data + array->length, value, valueLength);
	array->length += valueLength;
	return true;
}
bool Array_EmplaceBackSize(Array* array, const char* const value, size_t size) {
	if (!array || !value)
		return false;

	size_t newSize = array->length + size + 1;
	if (newSize >= array->reservedSize) {
		newSize = nearestMultipleOf(newSize, 8);
		if (!Array_Resize(array, newSize))
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

	assert(array->data);

	free(array->data);
	array->data = NULL;

	free(array);
	array = NULL;
}


struct StringArray {
	Array** data;
	size_t reservedSize;
	size_t length;
};

StringArray* StringArray_Create(size_t size) {
	StringArray* array = (StringArray*)calloc(1, sizeof(StringArray));
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Failed to allocate memory for StringArray");
#endif
		return NULL;
	}

	array->reservedSize = size + 1;
	array->length = 0;
	array->data = (Array**)calloc(1, sizeof(Array*));
	return array;
}

bool StringArray_Reserve(StringArray* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return false;
	}

	if (array->reservedSize <= size)
		return true;

	return StringArray_Resize(array, size);
}
bool StringArray_Resize(StringArray* array, size_t size) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return false;
	}

	if (array->reservedSize == size)
		return true;

	if (array->reservedSize < size) {
		Array** temp = array->data;
		array->reservedSize = size;
		array->data = (Array**)calloc(array->reservedSize, sizeof(Array*));
		if (!array->data)
			return false;

		memcpy(array->data, temp, array->length);
		free(temp);

		memset(array->data + array->length, 0, array->reservedSize - array->length);
		return true;
	}

	Array** temp = array->data;
	array->reservedSize = size + 1;
	array->data = (Array**)calloc(array->reservedSize, sizeof(Array*));
	if (!array->data)
		return false;

	memcpy(array->data, temp, size);
	free(temp);
	return true;
}

bool StringArray_PushBack(StringArray* array, const char* const value) {
	return StringArray_PushBackArray(array, Array_CreateFromString(value));
}
bool StringArray_PushBackArray(StringArray* array, const Array* const value) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return false;
	}

	if (array->length >= array->reservedSize) {
		size_t newSize = nearestMultipleOf(array->length + 2, 8);
		if (!StringArray_Resize(array, newSize))
			return false;
	}

	Array* copy = Array_Copy(value);
	array->data[array->length] = copy;
	array->length++;
	return true;
}

const Array* const StringArray_At(const StringArray* const array, size_t index) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return NULL;
	}

	if (index >= array->length) {
#ifdef _DEBUG
		LOG_ERR("Index out of bounds");
#endif
		return NULL;
	}

	return array->data[index];
}

size_t StringArray_GetSize(const StringArray* const array) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return -1;
	}
	return array->length;
}
size_t StringArray_GetReservedSize(const StringArray* const array) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return -1;
	}
	return array->reservedSize;
}

void StringArray_Destroy(StringArray* array) {
	if (!array) {
#ifdef _DEBUG
		LOG_ERR("Array is NULL");
#endif
		return;
	}

	for (size_t i = 0; i < array->length; i++) {
#ifdef _DEBUG
		LOG("Deleting element %llu in 0x%08" PRIX64 "...", i, (uintptr_t)array);
#endif
		Array_Destroy(array->data[i]);
		array->data[i] = NULL;
	}

	free(array->data);
	free(array);
	array = NULL;
}
