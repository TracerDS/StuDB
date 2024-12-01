#include <student.h>
#include <utils.h>
#include <names.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STUDENT_DATA_SEPARATOR ","

struct Student {
	Array* name;
	Array* surname;
	Array* address;
	Array* email;
	uint8_t age;
	uint16_t id;
};

Student* Student_Create(
	const Array* const name,
	const Array* const surname,
	const Array* const address,
	const Array* const email,
	uint8_t age,
	uint16_t id
) {
	Student* student = (Student*)calloc(1, sizeof(Student));
	if (!student)
		return NULL;

	student->name = Array_Copy(name);
	student->surname = Array_Copy(surname);
	student->address = Array_Copy(address);
	student->email = Array_Copy(email);

	if (!student->name || !student->surname || !student->address || !student->email) {
		return NULL;
	}

	student->age = age;
	student->id = id;

	return student;
}

Student* Student_CreateFromString(
	const char* const name,
	const char* const surname,
	const char* const address,
	const char* const email,
	uint8_t age,
	uint16_t id
) {
	return Student_Create(
		Array_CreateFromString(name),
		Array_CreateFromString(surname),
		Array_CreateFromString(address),
		Array_CreateFromString(email),
		age,
		id
	);
}

Student* Student_CreateRandom() {
	Student* student = (Student*)calloc(1, sizeof(Student));
	if (!student)
		return NULL;

	const Array* studentName = GetRandomName();
	size_t studentNameSize = Array_GetSize(studentName);
	const Array* studentSurname = GetRandomSurname();
	size_t studentSurnameSize = Array_GetSize(studentSurname);

	Array* studentEmail = Array_Create(studentNameSize + studentSurnameSize + 12);
	Array_EmplaceBackSize(studentEmail, Array_GetData(studentName), studentNameSize);
	Array_PushBack(studentEmail, '.');
	Array_EmplaceBackSize(studentEmail, Array_GetData(studentSurname), studentSurnameSize);
	Array_EmplaceBack(studentEmail, "@google.com");

	student->name = Array_Copy(studentName);
	student->surname = Array_Copy(studentSurname);
	
	student->address = Array_CreateFromString("Belize");
	student->email = studentEmail;

	student->age = (uint8_t)randomNumberBetween(12, 90);
	student->id = (uint8_t)randomNumberBetween(1000, 9999);

	return student;
}

Student* Student_CreateFromData(const char* const data) {
	if (!data)
		return NULL;

	size_t size = strlen(data);

	char* buffer = (char*)calloc(size + 1, sizeof(char));
	if (!buffer)
		return NULL;

	memcpy(buffer, data, size * sizeof(char));

	Student* student = (Student*)calloc(1, sizeof(Student));
	if (!student) {
		free(buffer);
		return NULL;
	}
	student->age = 0;
	student->id = 0;

	size_t index = 0;
	index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
	student->name = Array_CreateFromString(getFirstSubstringFromIndex(buffer, index));

	if (index != -1) {
		index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
		student->surname = Array_CreateFromString(getFirstSubstringFromIndex(buffer, index));
	}

	if (index != -1) {
		index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
		student->address = Array_CreateFromString(getFirstSubstringFromIndex(buffer, index));
	}

	if (index != -1) {
		index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
		student->email = Array_CreateFromString(getFirstSubstringFromIndex(buffer, index));
	}

	if (index != -1) {
		index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
		char* temp = getFirstSubstringFromIndex(buffer, index);
		if (temp) {
			student->age = (uint8_t)strtoul(temp, NULL, 10);
			free(temp);
		}
	}

	if (index != -1) {
		index = findSubstring(buffer + index, STUDENT_DATA_SEPARATOR);
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

const char* const Student_GetName(const Student* const student) {
	if (!student)
		return NULL;
	return Array_GetData(student->name);
}

const char* const Student_GetSurname(const Student* const student) {
	if (!student)
		return NULL;
	return Array_GetData(student->surname);
}

const char* const Student_GetAddress(const Student* const student) {
	if (!student)
		return NULL;
	return Array_GetData(student->address);
}

const char* const Student_GetEmail(const Student* const student) {
	if (!student)
		return NULL;
	return Array_GetData(student->email);
}

const Array* const Student_GetNameArray(const Student* const student) {
	if (!student)
		return NULL;
	return student->name;
}

const Array* const Student_GetSurnameArray(const Student* const student) {
	if (!student)
		return NULL;
	return student->surname;
}

const Array* const Student_GetAddressArray(const Student* const student) {
	if (!student)
		return NULL;
	return student->address;
}

const Array* const Student_GetEmailArray(const Student* const student) {
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


bool Student_SetName(Student* const student, const Array* const name) {
	if (!student || !name)
		return false;

	Array_Destroy(student->name);
	student->name = Array_Copy(name);
	return student->name != NULL;
}

bool Student_SetSurname(Student* const student, const Array* const surname) {
	if (!student)
		return false;

	Array_Destroy(student->name);
	student->surname = Array_Copy(surname);
	return student->surname != NULL;
}

bool Student_SetAddress(Student* const student, const Array* const address) {
	if (!student)
		return false;

	Array_Destroy(student->address);
	student->address = Array_Copy(address);
	return student->address != NULL;
}

bool Student_SetEmail(Student* const student, const Array* const email) {
	if (!student)
		return false;

	Array_Destroy(student->email);
	student->email = Array_Copy(email);
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

	Array_Destroy(student->name);
	Array_Destroy(student->surname);
	Array_Destroy(student->address);
	Array_Destroy(student->email);
	free(student);
	student = NULL;
}
