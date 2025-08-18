// main.cpp — запуск тритового процессора
#include <iostream>
#include <windows.h>
#include "core/trit.h"
#include "core/memory.h"
#include "core/CPU.h"
#include "utils/trit6.h"
#include "utils/utils.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Инициализация процессора
	memory mem(20);
	CPU cpu;
	cpu.attachmemory(&mem);
	
	registers[0] = tryte(trit::Plus, trit::Plus, trit::Plus);// регистр со значением 13 (+++)
	registers[1] = tryte(trit::Minus, trit::Minus, trit::Plus); // регистр со значением -11 (-++)
	registers[2] = tryte(trit::Plus, trit::Plus, trit::Plus); // регистр со значением -11 (-++)

	mem.set(0, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD
	mem.set(1, tryte(trit::Minus, trit::Minus, trit::Minus)); // Регистр 0, 
	mem.set(2, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1
	
	mem.set(3, tryte(trit::Zero, trit::Minus, trit::Minus)); // SUB
	mem.set(4, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1, 
	mem.set(5, tryte(trit::Minus, trit::Minus, trit::Plus)); // Регистр 2

	mem.set(6, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, останавливаем процессор обязательно

	// запускаем процессор
	cpu.run();

	i_trit6(0, 221);
	i_trit6(1, 219);
	i_trit6(2, 209);
	i_trit6(3, 10);

	std::cout << TRIT6[o_trit6(0)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(1)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(2)] << TRIT6[o_trit6(3)];
}