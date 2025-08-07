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
    memory mem(12);
    CPU cpu;
    cpu.attachmemory(&mem);
    
    cpu.registers[0] = tryte(trit::Zero, trit::Plus, trit::Plus);// регистр со значением 4 (0++)
    cpu.registers[1] = tryte(trit::Zero, trit::Plus, trit::Zero); // регистр со значением 3 (0+0)
    cpu.registers[20] = tryte(trit::Plus, trit::Plus, trit::Zero); // регистр со значением 12 (++0)

    // пишем команды в память. Сначало счетчик команды, потом команда или аргумент
    mem.set(0, tryte(trit::Plus, trit::Plus, trit::Zero)); // INC
	mem.set(1, tryte(trit::Plus, trit::Minus, trit::Plus)); // Регистр 20, получаем 13 (+++)

    mem.set(2, tryte(trit::Minus, trit::Minus, trit::Zero)); // DEC
    mem.set(3, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1, получаем 2 (0+-)

    mem.set(4, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD
	mem.set(5, tryte(trit::Minus, trit::Minus, trit::Minus)); // Регистр 0, получаем 7 (+-+)
	mem.set(6, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1

	mem.set(7, tryte(trit::Zero, trit::Minus, trit::Minus)); // SUB
    mem.set(8, tryte(trit::Plus, trit::Minus, trit::Plus)); // Регистр 20, получаем 5 (+--)
    mem.set(9, tryte(trit::Minus, trit::Minus, trit::Minus)); // Регистр 0

   // mem.set(10, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, останавливаем процессор обязательно

    // запускаем процессор
    cpu.run();
}