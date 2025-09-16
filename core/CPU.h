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

        case tryte(trit::Zero, trit::Zero, trit::Plus).raw(): { // 00+ HALT
            std::cout << "HALT: выполнение остановлено." << std::endl;
            halted = true;
            break;
        }

        case tryte(trit::Plus, trit::Plus, trit::Zero).raw(): { // ++0 INC R0 ++
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
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

        case tryte(trit::Plus, trit::Zero, trit::Minus).raw(): { // +0- STORE
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13; // номер регистра
            int addr = utils::toInt(memory_cpu->get(pc++));      // адрес в памяти
            if (reg < 26) {
                memory_cpu->set(addr, registers[reg]);
                std::cout << "STORE R" << reg << " → [" << addr << "]: " << registers[reg].toString() << std::endl;
            }
            break;
        }

        case tryte(trit::Minus, trit::Zero, trit::Plus).raw(): { // -0+ LOADM
            int reg_dst = utils::toInt(memory_cpu->get(pc++)) + 13; // куда грузим
            int reg_addr = utils::toInt(memory_cpu->get(pc++)) + 13; // откуда берём адрес
            if (reg_dst < 26 && reg_addr < 26) {
                int addr = utils::toInt(registers[reg_addr]);
                registers[reg_dst] = memory_cpu->get(addr);
                std::cout << "LOADM [" << addr << "] → R" << reg_dst << ": " << registers[reg_dst].toString() << std::endl;
            }
            break;
        }

        case tryte(trit::Minus, trit::Zero, trit::Minus).raw(): { // -0- STOREM
            int reg_src = utils::toInt(memory_cpu->get(pc++)) + 13; // откуда берём данные
            int reg_addr = utils::toInt(memory_cpu->get(pc++)) + 13; // где лежит адрес

            if (reg_src < 27 && reg_addr < 27) {
                int addr = utils::toInt(registers[reg_addr]); // достаём адрес из регистра
                memory_cpu->set(addr, registers[reg_src]);    // пишем в память
                std::cout << "STOREM R" << (reg_src - 13)
                    << " → mem[" << addr << "]: "
                    << registers[reg_src].toString() << std::endl;
            }
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

        case tryte(trit::Zero, trit::Minus, trit::Plus).raw(): { // 0-+ JMP
            tryte address = memory_cpu->get(pc++);
            int addr = utils::toInt(address);
            std::cout << "JMP → " << addr << std::endl;
            pc = addr;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Minus).raw(): { // --- NOT
            int reg = utils::toInt(memory_cpu->get(pc++)) + 13;
            if (reg < 26) registers[reg] = registers[reg].Not();
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
| №   | Инструкция | Назначение                           |
| --  | ---------- | ------------------------------------ |
| 000 | NOP        | Пустая операция                      |
| 00+ | HLT        | Остановка программы                  |
| ??? | RESET      | Сброс процессора                     |
| +0+ | LOAD       | Загрузка значения в регистр          |
| +0- | STORE      | Запись значения регистра в память    |
| -0+ | LOADM      | Загрузка из памяти в регистр         |
| -0- | STOREM     | Запись в память из регистра          |
| ++0 | INC        | Увеличение регистра на 1             |
| --0 | DEC        | Уменьшение регистра на 1             |
| 0++ | ADD        | Сложение                             |
| 0-- | SUB        | Вычитание                            |
| 0-+ | JMP        | Безусловный переход                  |
| ??? | JE         | Переход если равно                   |
| ??? | JNE        | Переход если не равно                |
| ??? | CMP        | Сравнение                            |
| ??? | PUSH       | Поместить в стек                     |
| ??? | POP        | Извлечь из стека                     |
| ??? | CALL       | Вызов подпрограммы                   |
| ??? | RET        | Возврат из подпрограммы              |
| ??? | MOV        | Копировать значение между регистрами |
| ??? | MUL        | Умножение                            |
| ??? | DIV        | Деление                              |
| ??? | AND        | Побитовое И                          |
| ??? | OR         | Побитовое ИЛИ                        |
| ??? | XOR        | Исключающее ИЛИ                      |
| --- | NOT        | Побитовое отрицание                  |

*/