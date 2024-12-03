#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <array.h>

#define MIN_ID_VALUE 1000
#define MAX_ID_VALUE 9999

#define MIN_AGE_VALUE 12
#define MAX_AGE_VALUE 120

typedef struct Student Student;

Student* Student_Create(
	const Array* const name,
	const Array* const surname,
	const Array* const address,
	const Array* const email,
	uint8_t age,
	uint16_t id
);
Student* Student_CreateFromString(
	const char* const name,
	const char* const surname,
	const char* const address,
	const char* const email,
	uint8_t age,
	uint16_t id
);

Student* Student_CreateRandom();
Student* Student_CreateFromData(const char* const data);

const char* const Student_GetName(const Student* const student);
const char* const Student_GetSurname(const Student* const student);
const char* const Student_GetAddress(const Student* const student);
const char* const Student_GetEmail(const Student* const student);

const Array* const Student_GetNameArray(const Student* const student);
const Array* const Student_GetSurnameArray(const Student* const student);
const Array* const Student_GetAddressArray(const Student* const student);
const Array* const Student_GetEmailArray(const Student* const student);

uint8_t Student_GetAge(const Student* const student);
uint16_t Student_GetID(const Student* const student);

bool Student_SetName(Student* const student, const Array* const name);
bool Student_SetSurname(Student* const student, const Array* const surname);
bool Student_SetAddress(Student* const student, const Array* const address);
bool Student_SetEmail(Student* const student, const Array* const email);
bool Student_SetAge(Student* const student, uint8_t age);
bool Student_SetID(Student* const student, uint16_t id);

void Student_Destroy(Student* student);