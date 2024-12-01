#include <utils.h>

#include <ctype.h>
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
	long fileSize = ftell(file);
	if (fileSize == -1) {
		fclose(file);
		return false;
	}
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

void appendFile(const char* const path, const char* const data) {
	FILE* file = fopen(path, "a");
	if (!file)
		return;

	fwrite(data, sizeof(char), strlen(data), file);
	fclose(file);
}

int randomNumberBetween(int from, int to) {
	return rand() % (to - from + 1) + from;
}

const char* toLower(char* const string) {
	return transform(string, tolower);
}
const char* toUpper(char* const string) {
	return transform(string, toupper);
}
const char* capitalize(char* const string) {
	size_t length = strlen(string);
	if (length == 0)
		return string;

	if (length > 1) {
		string[0] = toupper(string[0]);
	}

	for (size_t i = 1; i < length; i++) {
		string[i] = tolower(string[i]);
	}
	return string;
}
const char* transform(char* const string, TransformFunc function) {
	if (!string)
		return NULL;
	size_t length = strlen(string);
	for (size_t i = 0; i < length; i++) {
		string[i] = function(string[i]);
	}
	return string;
}