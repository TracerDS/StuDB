#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <array.h>
#include <assert.h>

#include <studentlist.h>
#include <utils.h>

struct StudentList {
	Student** students;
	size_t reservedSize;
	size_t length;
};

typedef bool(*SorterFunc_t)(StudentList* data, SortingFunction func);

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
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	size_t newSize = list->length + size;

	if (!StudentList_Reserve(list, newSize)) {
		return false;
	}

	for (size_t i = 0; i < size; i++) {
		Student* student = Student_CreateRandom();
		if (!student)
			return false;

		uint16_t id = min(MAX_ID_VALUE, max(MIN_ID_VALUE, Student_GetID(student))) - 1;

		while (StudentList_IsIDReserved(list, ++id));
		Student_SetID(student, id);

		StudentList_AddStudent(list, student);
	}
	return true;
}

bool StudentList_Reserve(StudentList* const list, size_t size) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	if (list->reservedSize > size)
		return true;

	return StudentList_Resize(list, size);
}
bool StudentList_Resize(StudentList* const list, size_t size) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	if (list->reservedSize > size)
		return true;

	size_t newSize = size + 1;
	Student** temp = realloc(list->students, newSize * sizeof(Student*));
	if (!temp) {
		return false;
	}

	list->students = temp;
	list->reservedSize = newSize;
	return true;
}

bool StudentList_AddStudent(StudentList* const list, Student* const value) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	assert(value && "Student is NULL");
	if (!value)
		return false;

	if (!StudentList_Reserve(list, nearestMultipleOf(list->length + 1, 8))) {
		return false;
	}

	uint16_t id = Student_GetID(value);
	if(id < MIN_ID_VALUE)
	while (StudentList_IsIDReserved(list, id)) {
		Student_SetID(value, id++);
	}

	list->students[list->length++] = value;
	return true;
}
bool StudentList_AddStudentsFromFile(StudentList* const list, const char* const filepath) {
	return StudentList_AddStudentsFromCSV(list, filepath, false);
}
bool StudentList_AddStudentsFromCSV(StudentList* const list, const char* const filepath, bool withHeader) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	size_t fileSize = 0;
	char* buffer = readFile(filepath, &fileSize);
	if (!buffer)
		return false;

	bool wasHeader = false;
	size_t offset = 0;
	while (offset <= fileSize) {
		size_t sizePos = findSubstring(buffer + offset, "\n");
		if (sizePos == -1 || sizePos >= fileSize) {
			sizePos = fileSize - offset;
		}
		if (!wasHeader && withHeader) {
			wasHeader = true;
			offset += ++sizePos;
			continue;
		}

		char* temp = (char*)calloc(sizePos + 1, sizeof(char));
		if (!temp) {
			free(buffer);
			return false;
		}
		memcpy(temp, buffer + offset, sizePos++);

		Student* student = Student_CreateFromData(temp);
		free(temp);
		if (!StudentList_AddStudent(list, student)) {
			free(buffer);
			return false;
		}
		offset += sizePos;
	}
	free(buffer);
	return true;
}

Student* StudentList_Get(const StudentList* const list, size_t index) {
	assert(list && "StudentList is NULL");
	if (!list)
		return NULL;

	assert(index >= list->length && "Index too large");
	if (index >= list->length)
		return NULL;

	return list->students[index];
}
size_t StudentList_GetSize(const StudentList* const list) {
	assert(list && "StudentList is NULL");
	if (!list)
		return -1;
	return list->length;
}
size_t StudentList_GetReservedSize(const StudentList* const list) {
	assert(list && "StudentList is NULL");
	if (!list)
		return -1;
	return list->reservedSize;
}

void quicksort(Student** students, size_t left, size_t right, SortingFunction sortingFunc) {
    size_t i = left;
    size_t j = right;
    Student* tmp;
    Student* pivot = students[(left + right) / 2];

    while (i <= j) {
        while (sortingFunc(students[i], pivot) < 0) {
            i++;
        }
        while (sortingFunc(students[j], pivot) > 0) {
            j--;
        }
        if (i <= j) {
            tmp = students[i];
            students[i] = students[j];
            students[j] = tmp;
            i++;
            j--;
        }
    }

    if (left < j) {
        quicksort(students, left, j, sortingFunc);
    }
    if (i < right) {
        quicksort(students, i, right, sortingFunc);
    }
}

void bubblesort(StudentList* list, SortingFunction func) {
	assert(list && "StudentList is NULL");
    if (!list)
		return;

	assert(func && "SortingFunction is NULL");
	if (!func)
		return;

    bool swapped;
    for (size_t i = 0; i < list->length - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < list->length - i - 1; j++) {
			if (func(list->students[j], list->students[j + 1]) <= 0)
				continue;
			
            Student* temp = list->students[j];
			list->students[j] = list->students[j + 1];
			list->students[j + 1] = temp;
            swapped = true;
        }
        if (!swapped) break;
    }
}

void StudentList_Remove(StudentList* list, const Student* const student) {
	assert(list && "StudentList is NULL");
	if (!list)
		return;
	
	size_t pos = 0;
	for (; pos < list->length; ++pos) {
		if (student != list->students[pos])
			continue;

		Student_Destroy(list->students[pos]);
		list->students[pos] = NULL;
		break;
	}
	
	// list -> length - 1 + 1
	Student** temp = malloc(nearestMultipleOf(list->length, 8) * sizeof(Student*));
	if (!temp)
		return;

	memcpy(temp, list->students, (pos - 1) * sizeof(Student*));
	memcpy(temp + pos, list->students + pos + 1, (list->length - pos - 1) * sizeof(Student*));
	free(list->students);
	list->students = temp;
}

int StudentList_CompareAge(const Student* const a, const Student* const b) {
	uint8_t ageA = Student_GetAge(a);
	uint8_t ageB = Student_GetAge(b);
	if (ageA < ageB) return -1;
	if (ageA > ageB) return 1;
	return 0;
}
int StudentList_CompareID(const Student* const a, const Student* const b) {
	uint16_t idA = Student_GetID(a);
	uint16_t idB = Student_GetID(b);
	if (idA < idB) return -1;
	if (idA > idB) return 1;
	return 0;
}

Student* StudentList_GetByID(const StudentList* const list, uint16_t id) {
	assert(list && "StudentList is NULL");
	if (!list)
		return NULL;

	for (size_t i = 0; i < list->length; i++) {
		Student* student = list->students[i];
		if (!student)
			continue;
		if (Student_GetID(student) == id)
			return student;
	}
	return NULL;
}
void quicksort_helper(Student** students, size_t left, size_t right, SortingFunction sortingFunc) {
	size_t i = left;
	size_t j = right;
	Student* tmp;
	Student* pivot = students[(i + j) / 2];

	while (i <= j) {
		while (sortingFunc(students[i], pivot) < 0) {
			i++;
		}
		while (sortingFunc(students[j], pivot) > 0) {
			j--;
		}
		if (i <= j) {
			tmp = students[i];
			students[i] = students[j];
			students[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j) {
		quicksort_helper(students, left, j, sortingFunc);
	}
	if (i < right) {
		quicksort_helper(students, i, right, sortingFunc);
	}
}
bool quicksort(StudentList* data, SortingFunction func) {
	if (!data || !func)
		return false;

	quicksort_helper(data->students, 0, data->length - 1, func);
	return true;
}

bool bubblesort(StudentList* data, SortingFunction func) {
	if (!data || !func)
		return false;

	bool swapped;
	for (size_t i = 0; i < data->length - 1; i++) {
		swapped = false;
		for (size_t j = 0; j < data->length - i - 1; j++) {
			if (func(data->students[j], data->students[j + 1]) <= 0)
				continue;

			Student* temp = data->students[j];
			data->students[j] = data->students[j + 1];
			data->students[j + 1] = temp;
			swapped = true;
		}
		if (!swapped) break;
	}
	return true;
}

bool StudentList_Sort(StudentList* list, SortingType type) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	if (list->length < 2) {
		return false;
	}

	SortingFunction sortingFunc;

	switch (type) {
		case SORTINGTYPE_AGE:
			sortingFunc = StudentList_CompareAge;
			break;
		case SORTINGTYPE_ID:
		case SORTINGTYPE_DEFAULT:
		default:
			sortingFunc = StudentList_CompareID;
			break;
	}
	SorterFunc_t sorter = quicksort;
	
	return sorter(list, sortingFunc);
}

bool StudentList_IsIDReserved(const StudentList* const list, uint16_t id) {
	assert(list && "StudentList is NULL");
	if (!list)
		return false;

	for (size_t i = 0; i < list->length; i++) {
		Student* student = list->students[i];
		if (!student)
			continue;
		if (Student_GetID(student) == id)
			return true;
	}
	return false;
}

void StudentList_Destroy(StudentList* list) {
	assert(list && "StudentList is NULL");

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