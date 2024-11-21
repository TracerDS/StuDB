#include <names.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

StringArray* g_names;
StringArray* g_surnames;

static const char* names[] = {
	"JAN",
	"ANDRZEJ",
	"MARCIN",
	"MAREK",
	"STEFAN",
	"KATARZYNA",
	"ROBERT",
	"MICHAŁ",
	"STANISŁAW",
	"JÓZEF",
	"ADAM",
	"PAWEŁ",
	"KRZYSZTOF",
	"JAKUB",
	"ZOFIA",
	"AGNIESZKA",
	"BARBARA",
	"MATEUSZ"
};

static const char* surnames[] = {
	"ŚWITAJ",
	"RZĄDŁO",
	"NOWAK",
	"BUZA",
	"SZEWCZYK",
	"WÓJCIK",
	"KOWALCZYK",
	"KRZYSIK",
	"PALUSZKIEWICZ",
	"ZIELINIEWICZ",
	"BORKIEWICZ",
	"WOŹNIAK",
	"SIWEK",
	"JASZCZKA",
	"FORNALIK",
	"MAZUR",
	"TRZMIEL",
	"WALENCIAK",
	"KRAWCZYK",
	"LENART",
	"BAS",
	"BUZA",
	"BYLICA",
	"STELMACH",
	"GROCHAL",
	"KOWAL",
	"CHROSTEK",
	"KOZDROŃ"
};
static const size_t namesCount = sizeof(names) / sizeof(names[0]);
static const size_t surnamesCount = sizeof(surnames) / sizeof(surnames[0]);

void InitNames() {
	if (g_names || g_surnames)
		return;

	g_names = StringArray_Create(namesCount);
	if (!g_names) {
		LOG_ERR("Failed to create names array");
		exit(EXIT_FAILURE);
		return; // This is unreachable
	}
	for (size_t i = 0; i < namesCount; i++) {
		StringArray_PushBack(g_names, names[i]);
	}

	g_surnames = StringArray_Create(surnamesCount);
	if (!g_surnames) {
		LOG_ERR("Failed to create surnames array");
		exit(EXIT_FAILURE);
		return; // This is unreachable
	}
	for (size_t i = 0; i < surnamesCount; i++) {
		StringArray_PushBack(g_surnames, surnames[i]);
	}
}

void DestroyNames() {
	if (g_names) {
		StringArray_Destroy(g_names);
		g_names = NULL;
	}
	if (g_surnames) {
		StringArray_Destroy(g_surnames);
		g_surnames = NULL;
	}
}

const StringArray* const GetNames() { return g_names; }
const StringArray* const GetSurnames() { return g_surnames; }

const Array* GetRandomName() {
	int random = randomNumberBetween(0, (int)StringArray_GetSize(g_names) - 1);
	return StringArray_At(g_names, random);
}
const Array* GetRandomSurname() {
	int random = randomNumberBetween(0, (int)StringArray_GetSize(g_surnames) - 1);
	return StringArray_At(g_surnames, random);
}