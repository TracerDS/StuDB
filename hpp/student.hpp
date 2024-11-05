#pragma once

#include <stdint.h>

typedef struct Student Student;

Student* Student_Create (
	const char* const name = NULL,
	const char* const surname = NULL,
	const char* const address = NULL,
	const char* const email = NULL,
	uint8_t age = 0,
	uint16_t id = 0
);

const char* Student_GetName(const Student* const student);
const char* Student_GetSurname(const Student* const student);
const char* Student_GetAddress(const Student* const student);
const char* Student_GetEmail(const Student* const student);
uint8_t Student_GetAge(const Student* const student);
uint16_t Student_GetID(const Student* const student);

bool Student_SetName(Student* student, const char* const name);
bool Student_GetSurname(Student* student, const char* const surname);
bool Student_GetAddress(Student* student, const char* const address);
bool Student_GetEmail(Student* student, const char* const email);
bool Student_GetAge(Student* student, uint8_t age);
bool Student_GetID(Student* student, uint16_t id);

void Student_Destroy(Student* student);