#pragma once

#include <student.hpp>

typedef struct StudentList StudentList;

StudentList* StudentList_Create();

bool StudentList_Reserve(StudentList* const list, size_t size);
bool StudentList_Resize(StudentList* const list, size_t size);

bool StudentList_PushBack(StudentList* const array, Student* const value);

Student* Array_Get(const StudentList* const list, size_t index);
size_t Array_GetSize(const StudentList* const list);
size_t Array_GetReservedSize(const StudentList* const list);

void StudentList_Destroy(StudentList* list);