#include <studentlist.h>
#include <utils.h>

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <array.h>

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

bool StudentList_GenerateRandom(StudentList* const list, size_t size) {
	if (!list)
		return false;

	if (list->length + size >= list->reservedSize) {
		if (!StudentList_Resize(list, list->length + size))
			return false;
	}

	for (size_t i = 0; i < size; i++) {
		Student* student = Student_CreateRandom();
		if (!student)
			return false;

		StudentList_AddStudent(list, student);
	}
	return true;
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

	size_t fileSize = 0;
	char* buffer = readFile(filepath, &fileSize);
	if (!buffer)
		return false;

	size_t offset = 0;
	while (offset <= fileSize) {
		size_t endOffset = findSubstring(buffer + offset, "\n");
		if (endOffset == -1)
			break;

		Array* arr = Array_Create(endOffset);
		if (!arr) {
			LOG_ERR("Failed to allocate array data");
			exit(EXIT_FAILURE);
			return false; // This is unreachable
		}

		if (!Array_EmplaceBackSize(arr, buffer + offset, endOffset)) {
			LOG_ERR("Failed to emplace data into array");
			Array_Destroy(arr);
			exit(EXIT_FAILURE);
			return false; // This is unreachable
		}

		Student* student = Student_CreateFromData(Array_GetData(arr));
		if (!student) {
			LOG_ERR("An error occured while creating student from data");
			Array_Destroy(arr);
			exit(EXIT_FAILURE);
			return false; // This is unrechable
		}
		Array_Destroy(arr);

		StudentList_AddStudent(list, student);
		
		offset += endOffset + 1;
	}
	free(buffer);
	return true;
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

	for (size_t i = 0; i < list->length; i++) {
		Student* student = list->students[i];
		if (!student)
			continue;
		Student_Destroy(student);
	}

	free(list->students);
	free(list);
	list = NULL;
}