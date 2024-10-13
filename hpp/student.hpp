#pragma once

#include <cstdint>
#include <string>

struct Student {
	std::string name;
	std::uint16_t age;
	std::uint16_t id;
};

Student* AddStudent(std::string name, std::uint8_t age, std::uint16_t id);
void RemoveStudent(const Student* const student);

void AddStudentsFromFile(const char* path);
void AddStudentsFromBuffer(const char* data);

void CreateStudentsList();
Student** GetStudents();
Student* GetStudent(std::uint16_t id);
uint32_t GetAmountOfStudents();
uint32_t GetStudentsSize();
void DeleteStudentsList();