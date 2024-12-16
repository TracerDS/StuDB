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
	if (false) {
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

			fprintf(stdout, "%d\n", ID);
			//fprintf(stdout, "%s,%s,%s,%s,%d,%d\n", name, surname, email, address, age, ID);
		}
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
			case '3': {
				printf("Z pliku? [T/N]: ");
				char fromFile;
				scanf_s(" %c", &fromFile, 1);
				fromFile = tolower(fromFile);

				switch (fromFile) {
					case 't':
						printf("Podaj ścieżkę do pliku: ");
						char path[MAX_PATH] = {0};
						scanf_s(" %s", &path, MAX_PATH);
						if (!StudentList_AddStudentsFromFile(g_studentList, path)) {
							fprintf(stderr, "Nie można wczytać studentów z pliku!\n");
							break;
						}
						printf("Wczytano studentów z pliku \"%s\"\n", path);
						break;
					case 'n':
						printf("No\n");
						break;
					default:
						fprintf(stderr, "Nieprawidłowy wybór!\n");
						break;
				}
				break;
			}
			case '4':
				size_t size = StudentList_GetSize(g_studentList);
				if (size == 0) {
					printf("Nie ma żadnych studentów\n");
					break;
				}
				for (size_t i = 0; i < size; i++) {
					Student* student = StudentList_Get(g_studentList, i);

					const char* name = Student_GetName(student);
					const char* surname = Student_GetSurname(student);
					const char* email = Student_GetEmail(student);
					const char* address = Student_GetAddress(student);
					uint8_t age = Student_GetAge(student);
					uint16_t ID = Student_GetID(student);

					printf("%d - %s %s, %s - %s (%d)\n",
						ID, name, surname, address, email, age
					);
				}
				break;
			case '5':
				return 0;
			default:
				fprintf(stderr, "Nieznana opcja!\n");
				break;
		}
	}

	return 0;
}