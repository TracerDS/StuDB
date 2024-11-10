#include <studentlist.h>
#include <utils.h>

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct StudentList {
	Student** students;
	size_t reservedSize;
	size_t length;
};

StudentList* StudentList_Create() {
	StudentList* studentList = (StudentList*)calloc(1, sizeof(StudentList));
	if (!studentList)
		return NULL;

	studentList->reservedSize = 1;
	studentList->length = 0;
	studentList->students = (Student**)calloc(studentList->reservedSize, sizeof(Student*));

	return studentList;
}

bool StudentList_Reserve(StudentList* const list, size_t size) {
	if (!list)
		return false;

	if (list->reservedSize <= size)
		return true;

	return StudentList_Resize(list, size);
}
bool StudentList_Resize(StudentList* const list, size_t size) {
	if (!list)
		return false;

	if (list->reservedSize == size)
		return true;

	Student** temp = list->students;
	list->reservedSize = size + (list->reservedSize < size ? 1 : 0);
	list->students = (Student**)calloc(list->reservedSize, sizeof(Student*));
	if (!list->students)
		return false;

	memcpy(list->students, temp, list->reservedSize < size ? list->length : size);
	free(temp);
	memset(list->students + list->length, 0, list->reservedSize - list->length);
	return true;
}

bool StudentList_AddStudent(StudentList* const list, Student* const value) {
	if (!list)
		return false;

	if (list->length >= list->reservedSize) {
		size_t newSize = nearestMultipleOf(list->length + 2, 8);
		if (!StudentList_Resize(list, newSize))
			return false;

		list->students[list->length++] = value;
		return true;
	}

	list->students[list->length++] = value;
	return true;
}
bool StudentList_AddStudentsFromFile(StudentList* const list, const char* const filepath) {
	if (!list)
		return false;

	FILE* file = fopen(filepath, "r");
	if (!file)
		return false;

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)calloc(fileSize + 1, sizeof(char));
	if (!buffer) {
		fclose(file);
		return;
	}
	size_t offset = 0;
	while (offset > fileSize) {
		offset = findSubstring(buffer + offset, '\n');
		size_t endOffset = findSubstringEnd(buffer + offset, '\n');
		
		char* temp = (char*)calloc(endOffset - offset + 1, sizeof(char));
		if(!temp) {
			free(buffer);
			fclose(file);
			return false;
		}
		memcpy(temp, buffer + offset, endOffset - offset);

		Student* student = Student_CreateFromData(temp);
		free(temp);
		if (!StudentList_AddStudent(list, student)) {
			free(buffer);
			fclose(file);
			return false;
		}
	}
	free(buffer);
}

Student* StudentList_Get(const StudentList* const list, size_t index) {
	if (!list)
		return NULL;

	if (index >= list->length)
		return NULL;

	return list->students[index];
}
size_t StudentList_GetSize(const StudentList* const list) {
	if (!list)
		return -1;
	return list->length;
}
size_t StudentList_GetReservedSize(const StudentList* const list) {
	if (!list)
		return -1;
	return list->reservedSize;
}

void StudentList_Destroy(StudentList* list) {
	if (!list)
		return;

	free(list->students);
	free(list);
	list = NULL;
}

/*

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
	char* buffer = (char*)calloc(size + 1, 1);
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
	g_students = (Student**)calloc(g_studentsAllocatedSize, sizeof(Student));
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
*/