#pragma once

#include <stddef.h>

#define LOG_STRING(x) "[" __DATE__ " " __TIME__ "] " x

#define LOG_HELPER_CUSTOM(stream, msg, ...) fprintf(stream, "[%s %s] " msg " - %s:%d\n", __DATE__, __TIME__, \
	##__VA_ARGS__, __func__, __LINE__)
#define LOG_HELPER(stream, msg, ...) LOG_HELPER_CUSTOM((stream), msg, ##__VA_ARGS__)
#define LOG_ERR(x, ...) LOG_HELPER(stderr, x, ##__VA_ARGS__)
#define LOG(x, ...) LOG_HELPER(stdout, x, ##__VA_ARGS__)

char* getFirstSubstringFromIndex(const char* const string, size_t index);
char* getFirstSubstring(const char* const string, char separator, size_t index);
size_t findSubstring(const char* const string, const char* const substring);
size_t findSubstringEnd(const char* const string, const char* const substring);
size_t nearestMultipleOf(size_t num, size_t multiple);
char* readFile(const char* const path, size_t* size);
int randomNumberBetween(int from, int to);