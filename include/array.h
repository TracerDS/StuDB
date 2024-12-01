#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Array Array;

#ifdef _DEBUG
enum {
	ARRAY_DEBUG_CONSTRUCTORS = (1 << 0),
	ARRAY_DEBUG_DESTRUCTORS = (1 << 1),
	ARRAY_DEBUG_RESIZE = (1 << 2),
	ARRAY_DEBUG_ALL = (1 << 3) - 1,
};
void Array_SetDebugMode(uint8_t value);
uint8_t Array_GetDebugMode();
#endif

Array* Array_Create(size_t size);
Array* Array_Copy(const Array* const array);
Array* Array_CreateFromString(const char* const string);
bool Array_Reserve(Array* array, size_t size);
bool Array_Resize(Array* array, size_t size);

bool Array_PushBack(Array* array, char value);
bool Array_EmplaceBack(Array* array, const char* const value);
bool Array_EmplaceBackSize(Array* array, const char* const value, size_t size);

char Array_At(const Array* const array, size_t index);
char* Array_GetData(const Array* const array);

size_t Array_GetSize(const Array* const array);
size_t Array_GetReservedSize(const Array* const array);

void Array_Destroy(Array* array);

// ------------------------------

typedef struct Vector Vector;

Vector* Vector_Create(size_t size);
bool Vector_Reserve(Vector* array, size_t size);
bool Vector_Resize(Vector* array, size_t size);

bool Vector_PushBack(Vector* array, const char* const value);
bool Vector_PushBackArray(Vector* array, const Array* const value);

const Array* const Vector_At(const Vector* const array, size_t index);

size_t Vector_GetSize(const Vector* const array);
size_t Vector_GetReservedSize(const Vector* const array);

void Vector_Destroy(Vector* array);
