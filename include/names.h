#pragma once

#include <array.h>

void InitNames();
void DestroyNames();

const Vector* const GetNames();
const Vector* const GetSurnames();

const Array* GetRandomName();
const Array* GetRandomSurname();