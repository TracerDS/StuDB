#include <student.h>
#include <studentlist.h>
#include <array.h>
#include <time.h>
#include <names.h>
#include <conio.h>
#include <windows.h>
#include <utils.h>

#define clrscr() printf("\x1b[2J\x1b[1;1H")

int main();
int MainFunc();

StudentList* g_studentList;

int main() {
	srand((unsigned int)time(NULL));
	int status = 0;
	InitNames();
	/*
	g_studentList = StudentList_Create();


	Array* a = Array_Create(3);
	Array_EmplaceBack(a, "abc");
	printf("%s\n", Array_GetData(a));
	Array_Destroy(a);

	//status = MainFunc();

	StudentList_Destroy(g_studentList);
	*/
	DestroyNames();
	return status;
}

int MainFunc() {
	if (!StudentList_AddStudentsFromFile(g_studentList, "data.sdbf")) {
		printf("Failed to allocate!");
		return 1;
	}

	size_t size = StudentList_GetSize(g_studentList);
	printf("Size: %llu\n", size);
	
	StudentList_GenerateRandom(g_studentList, 10);

	/*
	StudentList_AddStudentsFromFile(g_studentList, "data.sdbf");

	Student* student = Student_CreateFromString("Jan", "Kowalski", "ul. Kowalska 1", "email", 18, 9987);
	StudentList_AddStudent(g_studentList, student);

	*/

	for (int i = 0; i < size; i++) {
		Student* student = StudentList_Get(g_studentList, i);
		printf("Student: %s %s, %s, %s, %d, %d\n",
			Array_GetData(Student_GetName(student)),
			Array_GetData(Student_GetSurname(student)),
			Array_GetData(Student_GetAddress(student)),
			Array_GetData(Student_GetEmail(student)),
			Student_GetAge(student),
			Student_GetID(student)
		);
	}

	return 0;
}

/*
int MainFunc() {
	while (true) {
		clrscr();
		printf("/==================================\\\n");
		printf("|          BAZA STUDENCKA          |\n");
		printf("\\==================================/\n");
		printf("\nDostepne opcje:\n"
			"[1] Dodaj studenta\n"
			"[2] Usun studenta\n"
			"[3] Wczytaj studenta\n"
			"[4] Wyswietl studentow\n"
			"[5] Wyjdz\n"
		);
		char option = _getch();
		printf("\n");
		switch (option) {
			case '1':
			{
				printf("Imie: ");
				char name[20]{ 0 };
				scanf_s("%s", &name, 20);

				printf("Wiek: ");
				char ageStr[3]{};
				scanf_s("%s", &ageStr, 3);

				auto age = (uint8_t) strtoul(ageStr, nullptr, 10);
				if (age == 0) {
					printf("Nieprawidlowy wiek!\n");
					break;
				}

				printf("ID: ");
				char idStr[5]{ 0 };
				scanf_s("%s", &idStr, 5);
				auto id = (uint16_t) strtoul(idStr, nullptr, 10);
				if (id == 0) {
					printf("Nieprawidlowy numer ID!\n");
					break;
				}

				AddStudent(name, age, id);
				break;
			} case '2':
			{

				printf("ID: ");
				char idStr[5]{ 0 };
				scanf_s("%s", &idStr, 5);
				auto id = (uint16_t) strtoul(idStr, nullptr, 10);
				if (id == 0) {
					printf("Nieprawidlowy numer ID!\n");
					break;
				}

				auto student = GetStudent(id);
				if (!student) {
					printf("Nie ma studenta o ID r�wnym \"%s\"!", idStr);
					break;
				}
				RemoveStudent(student);

				break;
			}
			case '3':
			{
				printf("Podaj sciezke do pliku: ");
				char path[MAX_PATH]{ 0 };
				scanf_s("%s", &path, MAX_PATH);

				AddStudentsFromFile(path);
				break;
			}
			case '4':
			{
				auto size = GetStudentsSize();
				if (size == 0) {
					printf("Nie ma zadnych studentow\n");
					break;
				}
				printf("Lista studentow [%d]: \n", GetAmountOfStudents());

				auto* students = GetStudents();
				for (std::uint32_t i = 0; i < size; i++) {
					auto student = students[i];
					if (!student) continue;
					printf("- %s, %d [%d]\n", student->name.c_str(), student->age, student->id);
				}
				break;
			}
			case '5':
				return 0;
			default:
				printf("Nieznana opcja!\n");
				break;
		}
		printf("\nNacisnij dowolny przycisk aby powrocic");
		auto _ = _getch();
		clrscr();
	}
	return 0;
}
*/