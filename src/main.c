#include <student.h>
#include <studentlist.h>
#include <array.h>
#include <names.h>
#include <utils.h>

#include <time.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <inttypes.h>

#define clrscr() printf("\x1b[2J\x1b[1;1H")

int main();
int MainFunc();

StudentList* g_studentList;

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	srand((unsigned int)time(NULL));
	Array_SetDebugMode(ARRAY_DEBUG_ALL);
	Array_SetDebugMode(ARRAY_DEBUG_NOTHING);
	InitNames();

	int status = 0;
	g_studentList = StudentList_Create();
	
	status = MainFunc();

	StudentList_Destroy(g_studentList);
	DestroyNames();
	return status;
}

int MainFunc() {
	//StudentList_GenerateRandom(g_studentList, 100);
	StudentList_AddStudentsFromCSV(g_studentList, "students.csv", true);

	StudentList_Sort(g_studentList, SORTINGTYPE_DEFAULT);

	size_t size = StudentList_GetSize(g_studentList);

	for (size_t i = 0; i < size; i++) {
		Student* student = StudentList_Get(g_studentList, i);
		const char* name = Student_GetName(student);
		const char* surname = Student_GetSurname(student);
		const char* email = Student_GetEmail(student);
		const char* address = Student_GetAddress(student);
		uint8_t age = Student_GetAge(student);
		uint16_t ID = Student_GetID(student);

		fprintf(stdout, "%s,%s,%s,%s,%d,%d\n", name, surname, email, address, age, ID);
	}

	while (true) {
		clrscr();
		printf("/==================================\\\n");
		printf("|          BAZA STUDENCKA          |\n");
		printf("\\==================================/\n");
		printf("\nDostepne opcje:\n"
			"[1] Dodaj studenta\n"
			"[2] Usun studenta\n"
			"[3] Wczytaj studentów\n"
			"[4] Wyswietl studentów\n"
			"[5] Wyjdź\n"
		);
		char option = _getch();
		printf("\n");
		switch (option) {
			case '1':
			{
				char name[20];
				printf("Imię: ");
				scanf_s(" %s", name, 20);

				char surname[20];
				printf("Nazwisko: ");
				scanf_s(" %s", surname, 20);

				char address[20];
				printf("Adres: ");
				scanf_s(" %s", address, 20);

				char email[50];
				printf("E-Mail: ");
				scanf_s(" %s", email, 50);

				uint8_t age;
				{
					char ageStr[4] = { 0 };
					unsigned long ageTemp;
					do {
						printf("Wiek [%d - %d]: ", MIN_AGE_VALUE, MAX_AGE_VALUE);
						scanf_s(" %s", &ageStr, 3);
						ageTemp = strtoul(ageStr, NULL, 10);
						if (ageTemp < MIN_AGE_VALUE || ageTemp > MAX_AGE_VALUE) {
							fprintf(stderr, "Nieprawidłowy wiek!\n");
						}
					} while (ageTemp < MIN_AGE_VALUE || ageTemp > MAX_AGE_VALUE);
					age = (uint8_t)ageTemp;
				}

				uint16_t id;
				{
					char idStr[6] = { 0 };
					unsigned long idTemp;
					do {
						printf("ID [%d - %d]: ", MIN_ID_VALUE, MAX_ID_VALUE);
						scanf_s(" %s", &idStr, 5);
						idTemp = strtoul(idStr, NULL, 10);
						if (idTemp < MIN_ID_VALUE || idTemp > MAX_ID_VALUE) {
							fprintf(stderr, "Nieprawidłowe ID!\n");
						}
					} while (idTemp < MIN_ID_VALUE || idTemp > MAX_ID_VALUE);
					id = (uint16_t)idTemp;
				}
				
				Student* student = Student_CreateFromString(name, surname, address, email, age, id);
				if (!student || !StudentList_AddStudent(g_studentList, student)) {
					fprintf(stderr, "Nie można dodać studenta!\n");
					break;
				}

				printf("Dodano studenta:\n"
					"\t Imię: %s\n"
					"\t Nazwisko: %s\n"
					"\t Adres: %s\n"
					"\t E-Mail: %s\n"
					"\t Wiek: %d\n"
					"\t ID: %d\n",
					name, surname, address, email, age, id
				);				
				break;
			}
			case '2':
			{
				uint16_t id;
				{
					char idStr[6] = { 0 };
					unsigned long idTemp;
					do {
						printf("ID [%d - %d]: ", MIN_ID_VALUE, MAX_ID_VALUE);
						scanf_s(" %s", &idStr, 5);
						idTemp = strtoul(idStr, NULL, 10);
						if (idTemp < MIN_ID_VALUE || idTemp > MAX_ID_VALUE) {
							fprintf(stderr, "Nieprawidłowe ID!\n");
						}
					} while (idTemp < MIN_ID_VALUE || idTemp > MAX_ID_VALUE);
					id = (uint16_t)idTemp;
				}
				Student* student = StudentList_GetByID(g_studentList, id);
				if (!student) {
					fprintf(stderr, "Nie ma takiego studenta o ID równym \"%d\"!\n", id);
					break;
				}
				break;
			}
			case '5':
				return 0;
			default:
				fprintf(stderr, "Nieznana opcja!\n");
				break;
		}
		(void)_getch();
	}

	/*
	FILE* file = fopen("students.csv", "w");
	if (!file)
		return 1;

	fprintf(file, "Name,Surname,Email,Address,Age,ID\n");
	for (auto i = 0; i < size; i++) {
		Student* student = StudentList_Get(g_studentList, i);
		const char* name = Student_GetName(student);
		const char* surname = Student_GetSurname(student);
		const char* email = Student_GetEmail(student);
		const char* address = Student_GetAddress(student);
		uint8_t age = Student_GetAge(student);
		uint16_t ID = Student_GetID(student);

		fprintf(file, "%s,%s,%s,%s,%d,%d\n", name, surname, email, address, age, ID);
		fflush(file);
	}
	fclose(file);
	*/

	return 0;
}

/*
int MainFunc() {
	while (true) {
		switch (option) {
			case '2':
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
		}
		printf("\nNacisnij dowolny przycisk aby powrocic");
		auto _ = _getch();
		clrscr();
	}
	return 0;
}
*/