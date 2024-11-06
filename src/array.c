#include <array.h>
#include <utils.h>

#include <string.h>
#include <stdlib.h>

struct Array {
	char* data;
	size_t reservedSize;
	size_t length;
};

Array* Array_Create(size_t size) {
	Array* array = (Array*)calloc(1, sizeof(Array));
	if (!array)
		return NULL;

	array->reservedSize = size + 1;
	array->length = 0;
	array->data = (char*)calloc(array->reservedSize, sizeof(char));
	return array;
}

bool Array_Reserve(Array* array, size_t size) {
	if (!array)
		return false;

	if (array->reservedSize <= size)
		return true;

	return Array_Resize(array, size);
}

bool Array_Resize(Array* array, size_t size) {
	if (!array)
		return false;

	if (array->reservedSize == size)
		return true;

	if (array->reservedSize < size) {
		char* temp = array->data;
		array->reservedSize = size;
		array->data = (char*)calloc(array->reservedSize, sizeof(char));
		if (!array->data)
			return false;

		memcpy(array->data, temp, array->length);
		free(temp);

		memset(array->data + array->length, 0, array->reservedSize - array->length);
		return true;
	}

	char* temp = array->data;
	array->reservedSize = size + 1;
	array->data = (char*)calloc(array->reservedSize, sizeof(char));
	if (!array->data)
		return false;

	memcpy(array->data, temp, size);
	free(temp);
	return true;
}

bool Array_PushBack(Array* array, char value) {
	if (!array)
		return false;

	if (array->length >= array->reservedSize) {
		size_t newSize = nearestMultipleOf(array->length + 2, 8);
		if (!Array_Resize(array, newSize))
			return false;

		array->data[array->length++] = value;
		return true;
	}

	array->data[array->length++] = value;
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
	if (!array)
		return;

	free(array->data);
	free(array);
	array = NULL;
}