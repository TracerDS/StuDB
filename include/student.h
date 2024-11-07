#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Student Student;

Student* Student_Create(
	const char* const name,
	const char* const surname,
	const char* const address,
	const char* const email,
	uint8_t age,
	uint16_t id
);

Student* Student_CreateFromData(const char* const data);

const char* Student_GetName(const Student* const student);
const char* Student_GetSurname(const Student* const student);
const char* Student_GetAddress(const Student* const student);
const char* Student_GetEmail(const Student* const student);
uint8_t Student_GetAge(const Student* const student);
uint16_t Student_GetID(const Student* const student);

bool Student_SetName(Student* student, const char* const name);
bool Student_SetSurname(Student* student, const char* const surname);
bool Student_SetAddress(Student* student, const char* const address);
bool Student_SetEmail(Student* student, const char* const email);
bool Student_SetAge(Student* student, uint8_t age);
bool Student_SetID(Student* student, uint16_t id);

void Student_Destroy(Student* student);