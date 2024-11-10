#pragma once

#include <stddef.h>

char* getFirstSubstringFromIndex(const char* const string, size_t index);
char* getFirstSubstring(const char* const string, char separator, size_t index);
size_t findSubstring(const char* const string, const char* const substring);
size_t findSubstringEnd(const char* const string, const char* const substring);
size_t nearestMultipleOf(size_t num, size_t multiple);