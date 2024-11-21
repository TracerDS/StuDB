#include <utils.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char* getFirstSubstringFromIndex(const char* const string, size_t index) {
	if (index == -1)
		index = strlen(string);

	char* buffer = (char*)calloc(index + 1, sizeof(char));
	if (!buffer)
		return NULL;

	memcpy(buffer, string, index);
	return buffer;
}

char* getFirstSubstring(const char* const string, char separator, size_t index) {
	char separatorStr[2] = { separator, 0 };

	if (index == -1)
		index = strlen(string);

	size_t pos = findSubstring(string + index, separatorStr);
	if (pos == -1)
		return NULL;

	return getFirstSubstringFromIndex(string, pos);
}

size_t findSubstring(const char* const string, const char* const substring) {
	size_t stringLength = strlen(string);
	size_t substringLength = strlen(substring);

	for (size_t i = 0; i < stringLength; i++) {
		if (string[i] != substring[0])
			continue;

		size_t j = 0;
		for (; j < substringLength; j++) {
			if (string[i + j] != substring[j]) {
				break;
			}
		}

		if (j == substringLength) {
			return i;
		}
	}

	return -1;
}

size_t findSubstringEnd(const char* const string, const char* const substring) {
	size_t stringLength = strlen(string);
	size_t substringLength = strlen(substring);

	size_t pos = findSubstring(string, substring);
	if (pos == -1)
		return -1;

	return pos + substringLength;
}

size_t nearestMultipleOf(size_t num, size_t multiple) {
    return ((num + multiple - 1) / multiple) * multiple;
}

char* readFile(const char* const path, size_t* size) {
	FILE* file = fopen(path, "r");
	if (!file)
		return NULL;

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)calloc(fileSize + 1, sizeof(char));
	if (!buffer) {
		fclose(file);
		return NULL;
	}
	fread(buffer, sizeof(char), fileSize, file);
	fclose(file);

	if (size)
		*size = fileSize;
	return buffer;
}

int randomNumberBetween(int from, int to) {
	return rand() % (to - from + 1) + from;
}