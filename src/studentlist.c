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

bool StudentList_PushBack(StudentList* const list, Student* const value) {
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