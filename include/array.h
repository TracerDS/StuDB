#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Array Array;

#ifdef _DEBUG
void Array_SetDebugMode(uint8_t value);
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

typedef struct StringArray StringArray;

StringArray* StringArray_Create(size_t size);
bool StringArray_Reserve(StringArray* array, size_t size);
bool StringArray_Resize(StringArray* array, size_t size);

bool StringArray_PushBack(StringArray* array, const char* const value);
bool StringArray_PushBackArray(StringArray* array, const Array* const value);

const Array* const StringArray_At(const StringArray* const array, size_t index);

size_t StringArray_GetSize(const StringArray* const array);
size_t StringArray_GetReservedSize(const StringArray* const array);

void StringArray_Destroy(StringArray* array);
