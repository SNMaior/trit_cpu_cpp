// CPU.h — тритовый процессор с функцией записи числа в память
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "trit.h"
#include "memory.h"
#include "ALU.h"
#include "../utils/utils.h"
#include "registers.h"

// Класс CPU: содержит регистры и выполняет инструкции
class CPU {
public:
    bool halted = false;

    memory* memory_cpu = nullptr; // Указатель на подключённую память

    // Привязать память к CPU
    void attachmemory(memory* mem) {
        memory_cpu = mem;
    }

    // Инструкции
    void executeInstruction(const tryte& instruction) {
        switch (instruction.raw()) {
        case tryte(trit::Plus, trit::Zero, trit::Zero).raw(): { // +00 HALT
            std::cout << "HALT: выполнение остановлено." << std::endl;
            halted = true;
            break;
        }

        case tryte(trit::Plus, trit::Plus, trit::Zero).raw(): { // ++0 INC R0 ++
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
            std::cout << "reg → " << reg << std::endl;
            if (reg < 26) {
                std::pair<trit, tryte> result_pair = registers[reg].inc();
                EX = result_pair.first;
                tryte result = result_pair.second;
                registers[reg] = result;
            }
                std::cout << "INC reg → "<< static_cast<int>(EX) << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Zero).raw(): { // --0 DEC R0 --
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
            std::cout << "reg → " << reg << std::endl;
            if (reg < 26) { 
                std::pair<trit, tryte> result_pair = registers[reg].dec();
                EX = result_pair.first;
                tryte result = result_pair.second;
                registers[reg] = result;
            }
            std::cout << "DEC reg → " << static_cast<int>(EX) << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Plus, trit::Zero, trit::Plus).raw(): {// +0+ LOAD
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
            int addr = utils::toInt(memory_cpu->get(pc++));
            if (reg < 26)
                registers[reg] = memory_cpu->get(addr);
            std::cout << "LOAD [" << addr << "] → R0: " << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Plus, trit::Plus).raw(): { // 0++ ADD R0 + R1 = R0
            int regA = utils::toInt(memory_cpu->get(pc++)) + 13;
            int regB = utils::toInt(memory_cpu->get(pc++)) + 13;
            std::cout << "ADD regA → " << registers[regA].toString() << std::endl;
            std::cout << "ADD regB → " << registers[regB].toString() << std::endl;
            if (regA < 26 && regB < 26) {
                std::pair<trit, tryte> result_pair = registers[regA].add(registers[regB]);
                EX = result_pair.first;
                tryte result = result_pair.second;
                registers[regA] = result;
            }
            std::cout << "ADD R0 + R1 → R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Minus).raw(): { // 0-- SUB R0 - R1 = R0
            int regA = utils::toInt(memory_cpu->get(pc++)) + 13;
            int regB = utils::toInt(memory_cpu->get(pc++)) + 13;
            std::cout << "SUB regA → " << registers[regA].toString() << std::endl;
            std::cout << "SUB regB → " << registers[regB].toString() << std::endl;
            if (regA < 26 && regB < 26)            {
                std::pair<trit, tryte> result_pair = registers[regA].sub(registers[regB]);
                EX = result_pair.first;
                tryte result = result_pair.second;
                registers[regA] = result;
            }
            std::cout << "SUB R0 - R1 → R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Zero, trit::Plus).raw(): { // -0+ JMP
            tryte address = memory_cpu->get(pc++);
            int addr = utils::toInt(address);
            std::cout << "JMP → " << addr << std::endl;
            pc = addr;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Minus).raw(): { // --- negate
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
            if (reg < 26) registers[reg] = registers[reg].negate();
            break;
        }

        default: {
            std::cout << "Неизвестная инструкция: " << instruction.raw() << std::endl;
            break;
        }
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