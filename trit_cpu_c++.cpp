// main.cpp — запуск тритового процессора
#include <iostream>
#include <windows.h>
#include "trit.h"
#include "memory.h"
#include "CPU.h"
#include "trit6.h"
#include "utils.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "");

	// Инициализация процессора
    Memory mem(12);
    CPU cpu;
    cpu.attachMemory(&mem);
    
    cpu.registers[0] = Tryte(Trit::Zero, Trit::Plus, Trit::Plus);// регистр со значением 4 (0++)
    cpu.registers[1] = Tryte(Trit::Zero, Trit::Plus, Trit::Zero); // регистр со значением 3 (0+0)

    // пишем команды в память. Сначало счетчик команды, потом команда или аргумент
    mem.set(0, Tryte(Trit::Plus, Trit::Plus, Trit::Zero)); // INC, получаем 5 (+--)
    mem.set(1, Tryte(Trit::Plus, Trit::Zero, Trit::Plus)); // LOAD
    mem.set(2, Utils::fromInt(5)); // говорим, что нужное значение находиться на 5 шаге
	mem.set(3, Tryte(Trit::Minus, Trit::Zero, Trit::Plus)); // JMP
	mem.set(4, Utils::fromInt(6)); // говорим, что нужное перейти на 6 шаг
    mem.set(5, Utils::fromInt(10)); // значение для LOAD, которое попадет в R0, в данном случае это 10 (+0+)
    mem.set(6, Tryte(Trit::Zero, Trit::Plus, Trit::Plus)); // ADD, получаем 13 (+++) 
    mem.set(7, Tryte(Trit::Plus, Trit::Zero, Trit::Zero)); // HALT, останавливаем процессор обязательно

    // запускаем процессор
    cpu.run();

    std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // Выводим R0 для отладки
}

