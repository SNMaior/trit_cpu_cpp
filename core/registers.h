// registers.h - регистры
#pragma once
#include "trit.h"


tryte registers[27]; // общие регистры

tryte symreg[8][2]; // символьные регистры

trit EX; // регистр переполнения

trit logic; // логический регистр

size_t pc = 0;		   // Счётчик команд

tryte HI;
tryte LO;