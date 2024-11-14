#include <student.h>
#include <utils.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Student {
	char* name;
	char* surname;
	char* address;
	char* email;
	uint8_t age;
	uint16_t id;
};

Student* Student_Create(
	const char* const name,
	const char* const surname,
	const char* const address,
	const char* const email,
	uint8_t age,
	uint16_t id
) {
	Student* student = (Student*)calloc(1, sizeof(Student));
	if (!student)
		return NULL;

	size_t nameLen = strlen(name);
	size_t surnameLen = strlen(surname);
	size_t addressLen = strlen(address);
	size_t emailLen = strlen(email);

	student->name = (char*)calloc(nearestMultipleOf(nameLen + 1, 8), sizeof(char));
	student->surname = (char*)calloc(nearestMultipleOf(surnameLen + 1, 8), sizeof(char));
	student->address = (char*)calloc(nearestMultipleOf(addressLen + 1, 8), sizeof(char));
	student->email = (char*)calloc(nearestMultipleOf(emailLen + 1, 8), sizeof(char));
	
	student->age = age;
	student->id = id;

	return student;
}

Student* Student_CreateFromData(const char* const data) {
	if (!data)
		return NULL;

	size_t size = strlen(data);

	char* buffer = (char*)calloc(+1, sizeof(char));
	if (!buffer)
		return NULL;

	strncpy_s(buffer, size, data, size);

	Student* student = (Student*)calloc(1, sizeof(Student));
	if (!student) {
		free(buffer);
		return NULL;
	}
	size_t index = 0;

	index = findSubstring(buffer + index, ";");
	student->name = getFirstSubstringFromIndex(buffer, index);

	index = findSubstring(buffer + index, ";");
	student->surname = getFirstSubstringFromIndex(buffer, index);
	
	index = findSubstring(buffer + index, ";");
	student->address = getFirstSubstringFromIndex(buffer, index);

	index = findSubstring(buffer + index, ";");
	student->email = getFirstSubstringFromIndex(buffer, index);

	index = findSubstring(buffer + index, ";");
	{
		char* temp = getFirstSubstringFromIndex(buffer, index);
		if (temp) {
			student->age = (uint8_t)strtoul(temp, NULL, 10);
			free(temp);
		}
	}

	index = findSubstring(buffer + index, ";");
	{
		char* temp = getFirstSubstringFromIndex(buffer, index);
		if (temp) {
			student->id = (uint16_t)strtoul(temp, NULL, 10);
			free(temp);
		}
	}

	free(buffer);

	if (!student->name || !student->surname || !student->address || !student->email) {
		free(student);
		return NULL;
	}

	return student;
}

const char* Student_GetName(const Student* const student) {
	if (!student)
		return NULL;
	return student->name;
}

const char* Student_GetSurname(const Student* const student) {
	if (!student)
		return NULL;
	return student->surname;
}

const char* Student_GetAddress(const Student* const student) {
	if (!student)
		return NULL;
	return student->address;
}

const char* Student_GetEmail(const Student* const student) {
	if (!student)
		return NULL;
	return student->email;
}

uint8_t Student_GetAge(const Student* const student) {
	if (!student)
		return 0;
	return student->age;
}

uint16_t Student_GetID(const Student* const student) {
	if (!student)
		return 0;
	return student->id;
}


bool Student_SetName(Student* const student, const char* const name) {
	if (!student || !name)
		return false;

	free(student->name);
	size_t nameLen = strlen(name);
	student->name = (char*)calloc(nearestMultipleOf(nameLen + 1, 8), sizeof(char));
	return student->name != NULL;
}

bool Student_SetSurname(Student* const student, const char* const surname) {
	if (!student)
		return false;

	free(student->name);
	size_t surnameLen = strlen(surname);
	student->surname = (char*)calloc(nearestMultipleOf(surnameLen + 1, 8), sizeof(char));
	return student->surname != NULL;
}

bool Student_SetAddress(Student* const student, const char* const address) {
	if (!student)
		return false;

	free(student->address);
	size_t addressLen = strlen(address);
	student->address = (char*)calloc(nearestMultipleOf(addressLen + 1, 8), sizeof(char));
	return student->address != NULL;
}

bool Student_SetEmail(Student* const student, const char* const email) {
	if (!student)
		return false;

	free(student->email);
	size_t emailLen = strlen(email);
	student->email = (char*)calloc(nearestMultipleOf(emailLen + 1, 8), sizeof(char));
	return student->email != NULL;
}

bool Student_SetAge(Student* const student, uint8_t age) {
	if (!student)
		return false;

	student->age = age;
	return true;
}

bool Student_SetID(Student* const student, uint16_t id) {
	if (!student)
		return false;

	student->id = id;
	return true;
}


void Student_Destroy(Student* student) {
	if (!student)
		return;

	free(student->name);
	free(student->surname);
	free(student->address);
	free(student->email);
	free(student);
	student = NULL;
}
