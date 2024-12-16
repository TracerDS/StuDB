#include <utils.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#ifdef min
#	undef min
#endif
#ifdef max
#	undef max
#endif

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
	FILE* file = NULL;
	if (fopen_s(&file, path, "r") || !file)
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
	FILE* file = NULL;
	if(fopen_s(&file, path, "a") || !file);
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

int min(int a, int b) {
	return a < b ? a : b;
}
int max(int a, int b) {
	return a > b ? a : b;
}

char** split(char* string, const char delimiter, size_t* length) {
	char** result = 0;
	size_t count = 0;
	char* tmp = string;
	char* last_comma = 0;
	char delim[2];
	delim[0] = delimiter;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp) {
		if (delimiter == *tmp) {
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (string + strlen(string) - 1);

	/* Add space for terminating null string so caller
	   knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	char* context = NULL;
	if (result) {
		size_t idx = 0;
		char* token = strtok_s(string, delim, &context);

		while (token) {
			assert(idx < count);
			result[idx++] = _strdup(token);
			token = strtok_s(0, delim, &context);
		}
		assert(idx == count - 1);
		result[idx] = 0;
	}

	if (length)
		*length = count - 1;

	return result;
}

bool startsWith(const char* const string, const char* const with) {
	return strncmp(string, with, strlen(with)) == 0;
}

bool endsWith(const char* const string, const char* const with) {
	size_t sizeStr = strlen(string);
	size_t withLen = strlen(with);
	if (sizeStr < withLen)
		return false;
	return strncmp(string + sizeStr - withLen, with, withLen) == 0;
}