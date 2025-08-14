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
        case tryte(trit::Zero, trit::Zero, trit::Zero).raw(): { // 000 NOP
            std::cout << "NOP: ничего не делаем." << std::endl;
            break;
        }

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



/*
| №  | Инструкция | Назначение                           |
| -- | ---------- | ------------------------------------ |
| 0  | NOP        | Пустая операция                      |
| 1  | HLT        | Остановка программы                  |
| 2  | RESET      | Сброс процессора                     |
| 3  | LOAD       | Загрузка значения в регистр          |
| 4  | STORE      | Запись значения регистра в память    |
| 5  | LOADM      | Загрузка из памяти в регистр         |
| 6  | INC        | Увеличение регистра на 1             |
| 7  | DEC        | Уменьшение регистра на 1             |
| 8  | ADD        | Сложение                             |
| 9  | SUB        | Вычитание                            |
| 10 | JMP        | Безусловный переход                  |
| 11 | JE         | Переход если равно                   |
| 12 | JNE        | Переход если не равно                |
| 13 | CMP        | Сравнение                            |
| 14 | PUSH       | Поместить в стек                     |
| 15 | POP        | Извлечь из стека                     |
| 16 | CALL       | Вызов подпрограммы                   |
| 17 | RET        | Возврат из подпрограммы              |
| 18 | MOV        | Копировать значение между регистрами |
| 19 | MUL        | Умножение                            |
| 20 | DIV        | Деление                              |
| 21 | AND        | Побитовое И                          |
| 22 | OR         | Побитовое ИЛИ                        |
| 23 | XOR        | Исключающее ИЛИ                      |
| 24 | NOT        | Побитовое отрицание                  |
| 25 | SHL        | Сдвиг влево                          |
| 26 | SHR        | Сдвиг вправо                         |

*/