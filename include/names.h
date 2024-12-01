#pragma once

#include <array.h>

void InitNames();
void DestroyNames();

const StringArray* const GetNames();
const StringArray* const GetSurnames();

const Array* GetRandomName();
const Array* GetRandomSurname();