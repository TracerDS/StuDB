#pragma once

#include <student.h>
#include <stdbool.h>

typedef enum {
	SORTINGTYPE_DEFAULT,
	SORTINGTYPE_ID,
	SORTINGTYPE_NAME,
	SORTINGTYPE_SURNAME,
	SORTINGTYPE_EMAIL,
	SORTINGTYPE_ADDRESS,
	SORTINGTYPE_AGE
} SortingType;

typedef int(*SortingFunction)(const Student* const a, const Student* const b);

typedef struct StudentList StudentList;

StudentList* StudentList_Create();

bool StudentList_GenerateRandom(StudentList* const list, size_t size);

bool StudentList_Reserve(StudentList* const list, size_t size);
bool StudentList_Resize(StudentList* const list, size_t size);

bool StudentList_AddStudent(StudentList* const array, Student* const value);
bool StudentList_AddStudentsFromFile(StudentList* const array, const char* const filepath);
bool StudentList_AddStudentsFromCSV(StudentList* const array, const char* const filepath, bool withHeader);

Student* StudentList_Get(const StudentList* const list, size_t index);
size_t StudentList_GetSize(const StudentList* const list);
size_t StudentList_GetReservedSize(const StudentList* const list);

int StudentList_CompareAge(const Student* const a, const Student* const b);
int StudentList_CompareID(const Student* const a, const Student* const b);

bool StudentList_Sort(StudentList* list, SortingType type);
bool StudentList_IsIDReserved(const StudentList* const list, uint16_t id);

void StudentList_Destroy(StudentList* list);