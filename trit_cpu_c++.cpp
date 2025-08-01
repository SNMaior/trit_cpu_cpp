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
    memory mem(12);
    CPU cpu;
    cpu.attachmemory(&mem);
    
    cpu.registers[0] = tryte(trit::Zero, trit::Plus, trit::Plus);// регистр со значением 4 (0++)
    cpu.registers[1] = tryte(trit::Zero, trit::Plus, trit::Zero); // регистр со значением 3 (0+0)

    // пишем команды в память. Сначало счетчик команды, потом команда или аргумент
    mem.set(0, tryte(trit::Plus, trit::Plus, trit::Zero)); // INC, получаем 5 (+--)
    mem.set(1, tryte(trit::Plus, trit::Zero, trit::Plus)); // LOAD
    mem.set(2, utils::fromInt(5)); // говорим, что нужное значение находиться на 5 шаге
	mem.set(3, tryte(trit::Minus, trit::Zero, trit::Plus)); // JMP
	mem.set(4, utils::fromInt(6)); // говорим, что нужное перейти на 6 шаг
    mem.set(5, utils::fromInt(10)); // значение для LOAD, которое попадет в R0, в данном случае это 10 (+0+)
    mem.set(6, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD, получаем 13 (+++) 
    mem.set(7, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, останавливаем процессор обязательно

    // запускаем процессор
    cpu.run();
    tryte HALT = mem.get(5);
    std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // Выводим R0 для отладки
    std::cout << "Сет 5 = " << HALT.toString() << std::endl; // Выводим сет 5 для отладки
}

