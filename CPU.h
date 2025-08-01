// CPU.h — тритовый процессор с функцией записи числа в память
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "trit.h"
#include "memory.h"
#include "ALU.h"
#include "utils.h"

// Класс CPU: содержит регистры и выполняет инструкции
class CPU {
public:
    bool halted = false;

    tryte registers[27]; // 27 регистров (3 трита адресуют 3^3)
    size_t pc = 0;           // Счётчик команд
    memory* memory_cpu = nullptr; // Указатель на подключённую память

    // Привязать память к CPU
    void attachmemory(memory* mem) {
        memory_cpu = mem;
    }

    // Инструкции
    void executeInstruction(const tryte& instruction) {
        switch (instruction.raw()) {
        case tryte(trit::Plus, trit::Zero, trit::Zero).raw(): // +00 HALT
            std::cout << "HALT: выполнение остановлено." << std::endl;
            halted = true;
            break;

        case tryte(trit::Plus, trit::Plus, trit::Zero).raw(): // ++0 INC R0 ++
            registers[0] = registers[0].inc();
            std::cout << "INC R0 ? " << registers[0].toString() << std::endl;
            break;

        case tryte(trit::Minus, trit::Minus, trit::Zero).raw(): // --0 DEC R0 --
            registers[0] = registers[0].dec();
            std::cout << "DEC R0 ? " << registers[0].toString() << std::endl;
            break;

        case tryte(trit::Plus, trit::Zero, trit::Plus).raw(): {// +0+ LOAD
            tryte address = memory_cpu->get(pc++);
            int addr = utils::toInt(address);
            tryte value = memory_cpu->get(addr);
            registers[0] = value;
            std::cout << "LOAD [" << addr << "] ? R0: " << value.toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Plus, trit::Plus).raw(): // 0++ ADD R0 + R1 = R0
            std::cout << "ADD R0: " << registers[0].toString() << std::endl;
            std::cout << "ADD R1: " << registers[1].toString() << std::endl;
            registers[0] = registers[0].add(registers[1]);
            std::cout << "ADD R0 + R1 ? R0: " << " = " << registers[0].toString() << std::endl;
            break;

        case tryte(trit::Zero, trit::Minus, trit::Minus).raw(): // 0-- SUB R0 - R1 = R0
            std::cout << "SUB R0: " << registers[0].toString() << std::endl;
            std::cout << "SUB R1: " << registers[1].toString() << std::endl;
            registers[0] = registers[0].sub(registers[1]);
            std::cout << "SUB R0 - R1 ? R0: " << " = " << registers[0].toString() << std::endl;
            break;

        case tryte(trit::Minus, trit::Zero, trit::Plus).raw(): { // -0+ JMP
            tryte address = memory_cpu->get(pc++);
            int addr = utils::toInt(address);
            std::cout << "JMP ? " << addr << std::endl;
            pc = addr;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Minus).raw(): // --- negate
            registers[0] = registers[0].negate();
            break;

        default:
            std::cout << "Неизвестная инструкция: " << instruction.raw() << std::endl;
            break;
        }
    }

    // Основной цикл выполнения
    void run() {
        if (!memory_cpu) {
            std::cerr << "Ошибка: память не подключена!" << std::endl;
            return;
        }

        halted = false;
        while (!halted && pc < memory_cpu->size()) {
            tryte instr = memory_cpu->get(pc++);
            executeInstruction(instr);
        }
    }
};
