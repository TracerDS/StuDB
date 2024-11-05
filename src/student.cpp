#include <student.hpp>
#include <utils.hpp>

#include <stdlib.h>

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
		return NULL;
	return student->age;
}

uint16_t Student_GetID(const Student* const student) {
	if (!student)
		return NULL;
	return student->id;
}


bool Student_SetName(Student* student, const char* const name) {
	if (!student || !name)
		return false;

	free(student->name);
	size_t nameLen = strlen(name);
	student->name = (char*)calloc(nearestMultipleOf(nameLen + 1, 8), sizeof(char));
	return student->name != NULL;
}

bool Student_GetSurname(Student* student, const char* const surname) {
	if (!student)
		return false;

	free(student->name);
	size_t surnameLen = strlen(surname);
	student->surname = (char*)calloc(nearestMultipleOf(surnameLen + 1, 8), sizeof(char));
	return student->surname != NULL;
}

bool Student_GetAddress(Student* student, const char* const address) {
	if (!student)
		return false;

	free(student->address);
	size_t addressLen = strlen(address);
	student->address = (char*)calloc(nearestMultipleOf(addressLen + 1, 8), sizeof(char));
	return student->address != NULL;
}

bool Student_GetEmail(Student* student, const char* const email) {
	if (!student)
		return false;

	free(student->email);
	size_t emailLen = strlen(email);
	student->email = (char*)calloc(nearestMultipleOf(emailLen + 1, 8), sizeof(char));
	return student->email != NULL;
}

bool Student_GetAge(Student* student, uint8_t age) {
	if (!student)
		return false;

	student->age = age;

	return true;
}

bool Student_GetID(Student* student, uint16_t id) {
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
	free(student);
	student = NULL;
}


Student** g_students{ nullptr };
uint32_t g_studentsSize{ 0 };
uint32_t g_studentsAllocatedSize{ 0 };

void AddStudentsFromFile(const char* path) {
	FILE* file;
	fopen_s(&file, path, "r");
	if (!file)
		return;

	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return;
	}
	auto size = ftell(file);
	if (size == -1l) {
		fclose(file);
		return;
	}
	rewind(file);
	char* buffer = (char*) calloc(size + 1, 1);
	if (!buffer)
		return;

	fread(buffer, 1, size - 1, file);
	fclose(file);
	AddStudentsFromBuffer(buffer);
	free(buffer);
}


void AddStudentsFromBuffer(const char* data) {
	auto lines = splitString(data, "\n");
	for (const auto& line : lines) {
		auto data = splitString(line, ",");
		if (data.size() != 3) continue;

		auto age = (std::uint8_t)strtoul(data[1].c_str(), nullptr, 10);
		if (age == 0 || age == ULONG_MAX)
			continue;

		auto id = (std::uint16_t)strtoul(data[2].c_str(), nullptr, 10);
		if (id == 0 || id == ULONG_MAX)
			continue;

		AddStudent(data[0], age, id);
	}
}


void CreateStudentsList() {
	if (g_students)
		return;
	g_studentsAllocatedSize = 8;
	g_studentsSize = 0;
	g_students = (Student**) calloc(g_studentsAllocatedSize, sizeof(Student));
}

Student** GetStudents() { return g_students; }
Student* GetStudent(std::uint16_t id) {
	for (uint32_t i = 0; i < g_studentsAllocatedSize; i++) {
		Student* student = g_students[i];
		if (!student || student->id != id) continue;
		return student;
	}
	return nullptr;
}

uint32_t GetStudentsSize() { return g_studentsSize; }
uint32_t GetAmountOfStudents() {
	uint32_t out{ 0 };
	for (uint32_t i = 0; i < g_studentsAllocatedSize; i++) {
		if (!g_students[i]) continue;
		out++;
	}
	return out;
}

void DeleteStudentsList() {
	if (!g_students)
		return;

	auto temp = g_students;
	for (uint32_t i = 0; i < g_studentsAllocatedSize; i++) {
		Student* student = g_students[i];
		if (!student)
			continue;

		free(student);
		student = nullptr;
	}
	free(g_students);
	g_students = nullptr;
}