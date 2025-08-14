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
	//memory mem(20);
	//CPU cpu;
	//cpu.attachmemory(&mem);
	
	//registers[0] = tryte(trit::Plus, trit::Plus, trit::Plus);// регистр со значением 13 (+++)
	//registers[1] = tryte(trit::Minus, trit::Minus, trit::Plus); // регистр со значением -11 (-++)
	//registers[2] = tryte(trit::Plus, trit::Plus, trit::Plus); // регистр со значением -11 (-++)

	//mem.set(0, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD
	//mem.set(1, tryte(trit::Minus, trit::Minus, trit::Minus)); // Регистр 0, 
	//mem.set(2, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1
	//
	//mem.set(3, tryte(trit::Zero, trit::Minus, trit::Minus)); // SUB
	//mem.set(4, tryte(trit::Minus, trit::Minus, trit::Zero)); // Регистр 1, 
	//mem.set(5, tryte(trit::Minus, trit::Minus, trit::Plus)); // Регистр 2

	//mem.set(6, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, останавливаем процессор обязательно

	//// запускаем процессор
	//cpu.run();

	//i_trit6(0, 221);
	//i_trit6(1, 219);
	//i_trit6(2, 209);
	//i_trit6(3, 10);

	//std::cout << TRIT6[o_trit6(0)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(1)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(2)] << TRIT6[o_trit6(3)];

    // Создаём память на 20 трайтов
    memory mem(20);

    // Создаём CPU и подключаем память
    CPU cpu;
    cpu.attachmemory(&mem);

    // 1. Положим в память[13] значение 123
    mem.set(13, tryte(trit::Minus, trit::Minus, trit::Zero));

    // 2. В регистр R2 положим адрес 8 (для LOADM)
    registers[2] = tryte(trit::Plus, trit::Zero, trit::Minus);

    // === LOAD R0, [13] ===
    mem.set(0, tryte(trit::Plus, trit::Zero, trit::Plus)); // +0+ (код LOAD)
    mem.set(1, tryte(trit::Minus, trit::Minus, trit::Minus)); // регистр R0
    mem.set(2, tryte(trit::Plus, trit::Plus, trit::Plus));      // адрес 13

    // === STORE R0, [10] ===
    mem.set(3, tryte(trit::Plus, trit::Zero, trit::Minus)); // +0- (код STORE)
    mem.set(4, tryte(trit::Minus, trit::Minus, trit::Minus)); // регистр R0
    mem.set(5, tryte(trit::Plus, trit::Zero, trit::Plus));     // адрес 10

    // === LOADM R1, R2 ===
    mem.set(6, tryte(trit::Plus, trit::Zero, trit::Zero)); // +00 (код LOADM)
    mem.set(7, tryte(trit::Minus, trit::Minus, trit::Zero)); // регистр R1 (куда грузим)
    mem.set(8, tryte(trit::Minus, trit::Minus, trit::Plus)); // регистр R2 (в нём адрес)

    // === HALT ===
    mem.set(9, tryte(trit::Zero, trit::Zero, trit::Plus)); // HALT

    // Запускаем CPU
    cpu.run();

    // После выполнения:
    std::cout << "R0 = " << registers[0].toString() << std::endl;
    std::cout << "R1 = " << registers[1].toString() << std::endl;
    std::cout << "mem[10] = " << mem.get(10).toString() << std::endl;
    std::cout << "mem[8] = " << mem.get(8).toString() << std::endl;
    std::cout << "mem[13] = " << mem.get(13).toString() << std::endl;
}