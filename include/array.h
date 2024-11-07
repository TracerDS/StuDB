#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Array Array;

Array* Array_Create(size_t size);
bool Array_Reserve(Array* array, size_t size);
bool Array_Resize(Array* array, size_t size);

bool Array_PushBack(Array* array, char value);
bool Array_EmplaceBack(Array* array, const char* const value);

char* Array_GetData(const Array* const array);
size_t Array_GetSize(const Array* const array);
size_t Array_GetReservedSize(const Array* const array);

void Array_Destroy(Array* array);
