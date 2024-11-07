#include <utils.h>

#include <string.h>
#include <stdlib.h>

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

	size_t pos = findSubstring(string + index, separatorStr, index);
	if (pos == -1)
		return NULL;

	return getFirstSubstringFromIndex(string, pos);
}

size_t findSubstring(const char* const string, const char* const substring, size_t index) {
	size_t stringLength = strlen(string);
	size_t substringLength = strlen(substring);

	if (index == -1)
		index = 0;

	for (size_t i = 0; i < stringLength - index; i++) {
		if (string[index + i] != substring[0])
			continue;

		size_t j = 0;
		for (; j < substringLength; j++) {
			if (string[index + i + j] != substring[j]) {
				break;
			}
		}

		if (j == substringLength) {
			return i;
		}
	}

	return -1;
}

size_t nearestMultipleOf(size_t num, size_t multiple) {
    return ((num + multiple - 1) / multiple) * multiple;
}