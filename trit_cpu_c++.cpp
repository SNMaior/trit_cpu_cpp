// main.cpp — запуск тритового процессора
#include <iostream>
#include <windows.h>
#include "core/trit.h"
#include "core/memory.h"
#include "core/CPU.h"
#include "utils/trit6.h"
#include "utils/utils.h"

#include "core/registers.h"

trit EX = trit::Zero;// флаг переполнения
trit LG= trit::Zero;// логический флаг
trit SP = trit::Zero;// флаг стека

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Инициализация процессора
	Memory mem;
	Registers registers;
	CPU cpu(registers);
	cpu.attachmemory(&mem);
	Wide_Reg wreg;
	/*
	registers[tryte(trit::Minus, trit::Minus, trit::Minus)] = tryte(trit::Plus, trit::Plus, trit::Plus);// регистр со значением 13 (+++)
	registers[tryte(trit::Minus, trit::Minus, trit::Zero)] = tryte(trit::Minus, trit::Minus, trit::Plus); // регистр со значением -11 (--+)
	registers[tryte(trit::Minus, trit::Minus, trit::Plus)] = tryte(trit::Plus, trit::Plus, trit::Plus); // регистр со значением 13 (+++)
	
	std::cout << registers[tryte(trit::Minus, trit::Minus, trit::Minus)].toString() << std::endl;
	std::cout << registers[tryte(trit::Minus, trit::Minus, trit::Zero)].toString() << std::endl;
	std::cout << registers[tryte(trit::Minus, trit::Minus, trit::Plus)].toString() << std::endl;
	*/
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Minus, trit::Zero) }, tryte(trit::Zero, trit::Plus, trit::Zero)); // MOV -1, R0, 13
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Minus, trit::Plus) }, trit::Minus);
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Zero, trit::Minus) }, tryte(trit::Minus, trit::Minus, trit::Minus));
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Zero, trit::Zero) }, tryte(trit::Plus, trit::Plus, trit::Zero));
	
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Zero, trit::Plus) }, tryte(trit::Zero, trit::Plus, trit::Zero)); // MOV -1, R1, -11
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Plus, trit::Minus) }, trit::Minus);
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Plus, trit::Zero) }, tryte(trit::Minus, trit::Minus, trit::Zero));
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Minus, trit::Plus, trit::Plus) }, tryte(trit::Zero, trit::Plus, trit::Minus));

	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Zero, trit::Minus, trit::Minus) }, tryte(trit::Zero, trit::Plus, trit::Minus)); // CMP R0, R1
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Zero, trit::Minus, trit::Zero) }, tryte(trit::Minus, trit::Minus, trit::Minus));
	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Zero, trit::Minus, trit::Plus) }, tryte(trit::Minus, trit::Minus, trit::Zero));

	mem.set(pc{ tryte(trit::Minus, trit::Minus, trit::Minus), tryte(trit::Zero, trit::Zero, trit::Minus) }, tryte(trit::Zero, trit::Zero, trit::Plus)); // HALT, останавливаем процессор обязательно

	// запускаем процессор
	cpu.run();

	i_trit6(0, 221);
	i_trit6(1, 219);
	i_trit6(2, 209);
	i_trit6(3, 10);

	std::cout << TRIT6[o_trit6(0)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(1)] << TRIT6[o_trit6(3)] << TRIT6[o_trit6(2)] << TRIT6[o_trit6(3)];
}