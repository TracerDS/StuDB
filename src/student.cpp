#include <student.hpp>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <utils.hpp>

Student** g_students{ nullptr };
uint32_t g_studentsSize{ 0 };
uint32_t g_studentsAllocatedSize{ 0 };

Student* AddStudent(std::string name, uint8_t age, uint16_t id) {
	if (!g_students)
		return nullptr;

	// Je¿eli dotarliœmy do rozmiaru tablicy
	// to zallokuj wiêcej miejsca
	if (g_studentsSize >= g_studentsAllocatedSize) {
		g_studentsSize = g_studentsAllocatedSize;

		// Allokujemy tymczasow¹ tablicê
		Student** temp = (Student**) calloc(g_studentsAllocatedSize, sizeof(Student));
		// Je¿eli nie uda³o siê zallokowaæ tablicy to zwróæ nullptr
		if (!temp)
			return nullptr;

		// Robimy kopiê elementów ze starej tablicy
		memcpy(temp, g_students, g_studentsAllocatedSize);

		// Allokujemy now¹ tablicê
		free(g_students);
		g_students = (Student**) calloc(g_studentsAllocatedSize + 1, sizeof(Student));
		// Je¿eli nie uda³o siê zallokowaæ tablicy to zwróæ nullptr
		if (!g_students)
			return nullptr;

		memcpy(g_students, temp, g_studentsAllocatedSize++);
	}
	Student* student = (Student*) calloc(1, sizeof(Student));
	if (!student) return nullptr;

	student->name = std::move(name);
	student->age = age;
	student->id = id;

	g_students[g_studentsSize++] = student;
	return student;
}

void RemoveStudent(const Student* const student) {
	if (!student)
		return;

	if (!g_students)
		return;

	for (std::uint32_t i = 0; i < g_studentsSize; i++) {
		Student* currentStudent = g_students[i];
		if (currentStudent != student)
			continue;

		free(&currentStudent);
		currentStudent = nullptr;
	}
}

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