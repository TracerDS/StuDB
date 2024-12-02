#pragma once

#include <stddef.h>

#ifdef min
#	undef min
#endif
#ifdef max
#	undef max
#endif

#define LOG_STRING(x) "[" __DATE__ " " __TIME__ "] " x

#define LOG_HELPER_CUSTOM(stream, msg, ...) fprintf(stream, LOG_STRING(msg) " - %s:%d\n", \
	##__VA_ARGS__, __func__, __LINE__)
#define LOG_HELPER(stream, msg, ...) LOG_HELPER_CUSTOM((stream), msg, ##__VA_ARGS__)
#define LOG_ERR(x, ...) LOG_HELPER(stderr, x, ##__VA_ARGS__)
#define LOG(x, ...) LOG_HELPER(stdout, x, ##__VA_ARGS__)

typedef int(TransformFunc)(int);

char* getFirstSubstringFromIndex(const char* const string, size_t index);
char* getFirstSubstring(const char* const string, char separator, size_t index);
size_t findSubstring(const char* const string, const char* const substring);
size_t findSubstringEnd(const char* const string, const char* const substring);

size_t nearestMultipleOf(size_t num, size_t multiple);

char* readFile(const char* const path, size_t* size);
void appendFile(const char* const path, const char* const data);

int randomNumberBetween(int from, int to);

const char* toLower(char* const string);
const char* toUpper(char* const string);
const char* capitalize(char* const string);
const char* transform(char* const string, TransformFunc function);

int min(int a, int b);
int max(int a, int b);