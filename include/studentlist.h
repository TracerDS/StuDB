#pragma once

#include <student.h>
#include <stdbool.h>

typedef struct StudentList StudentList;

StudentList* StudentList_Create();

bool StudentList_Reserve(StudentList* const list, size_t size);
bool StudentList_Resize(StudentList* const list, size_t size);

bool StudentList_AddStudent(StudentList* const array, Student* const value);
bool StudentList_AddStudentsFromFile(StudentList* const array, const char* const filepath);

Student* StudentList_Get(const StudentList* const list, size_t index);
size_t StudentList_GetSize(const StudentList* const list);
size_t StudentList_GetReservedSize(const StudentList* const list);

void StudentList_Destroy(StudentList* list);