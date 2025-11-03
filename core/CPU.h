// CPU.h — тритовый процессор с функцией записи числа в память
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "trit.h"
#include "memory.h"
#include "ALU.h"
//#include "../utils/utils.h"
#include "registers.h"

// Класс CPU: содержит регистры и выполняет инструкции
class CPU {
public:
    bool halted = false;

    Memory* memory_cpu = nullptr; // Указатель на подключённую память

    // Привязать память к CPU
    void attachmemory(Memory* mem) {
        memory_cpu = mem;
    }

    pc PC;
    //wide_reg WR;
    Registers& registers;
    CPU(Registers& regs) : registers(regs) {}
    Wide_Reg wreg;

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
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            auto [EX, result] = registers[reg].inc();
            registers[reg] = result;
            std::cout << "INC reg > "<< static_cast<int>(EX) << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Zero).raw(): { // --0 DEC R0 --
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            auto [EX, result] = registers[reg].dec();
            registers[reg] = result;
            std::cout << "DEC reg > " << static_cast<int>(EX) << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Plus, trit::Zero, trit::Plus).raw(): {// +0+ LOAD
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            pc addr = memory_cpu->get(PC.inc()).asPc();
            registers[reg] = memory_cpu->get(addr).asTryte();
            std::cout << "LOAD [" << addr.HI.toString() << addr.LO.toString() << "] > R0: " << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Plus, trit::Zero, trit::Minus).raw(): { // +0- STORE
            tryte reg = memory_cpu->get(PC.inc()).asTryte(); // номер регистра
            pc addr = memory_cpu->get(PC.inc()).asPc();      // адрес в памяти
            memory_cpu->set(addr, registers[reg]);
            std::cout << "STORE R" << reg.toString() << " > [" << addr.HI.toString() << addr.LO.toString() << "]: " << registers[reg].toString() << std::endl;
            break;
        }

       case tryte(trit::Minus, trit::Zero, trit::Plus).raw(): { // -0+ LOADM
            tryte reg_dst = memory_cpu->get(PC.inc()).asTryte(); // куда грузим
            pc addr = memory_cpu->get(wreg.wrPc()).asPc(); // откуда берём адрес
            registers[reg_dst] = memory_cpu->get(addr).asTryte();
            std::cout << "LOADM [" << addr.HI.toString() << addr.LO.toString() << "] > R" << reg_dst.toString() << ": " << registers[reg_dst].toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Zero, trit::Minus).raw(): { // -0- STOREM
            tryte reg_src = memory_cpu->get(PC.inc()).asTryte(); // откуда берём данные
            pc addr = memory_cpu->get(wreg.wrPc()).asPc();; // где лежит адрес
            memory_cpu->set(addr, registers[reg_src]);    // пишем в память
            std::cout << "STOREM R" << reg_src.toString() << " > mem[" << addr.HI.toString() << addr.LO.toString() << "]: " << registers[reg_src].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Plus, trit::Plus).raw(): { // 0++ ADD R0 + R1 = R0
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "ADD regA > " << registers[regA].toString() << std::endl;
            std::cout << "ADD regB > " << registers[regB].toString() << std::endl;
            auto [EX, result] = registers[regA].add(registers[regB]);
            registers[regA] = result;
            std::cout << "ADD R0 + R1 > R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Minus).raw(): { // 0-- SUB R0 - R1 = R0
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "SUB regA > " << registers[regA].toString() << std::endl;
            std::cout << "SUB regB > " << registers[regB].toString() << std::endl;
            auto [EX, result] = registers[regA].sub(registers[regB]);
            registers[regA] = result;
            std::cout << "SUB R0 - R1 > R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Plus).raw(): { // 0-+ JMP
            pc addr = memory_cpu->get(PC.inc()).asPc();
            std::cout << "JMP > " << addr.HI.toString() << addr.LO.toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Minus).raw(): { // --- NOT
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            registers[reg] = registers[reg].Not();
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
        std::cout << "Запуск процессора..." << std::endl;
        std::cout << "Начальный PC: " << PC.HI.toString() << PC.LO.toString() << std::endl;

        halted = false;
        while (!halted) {
            tryte instr = memory_cpu->get(PC.inc()).asTryte();
            std::cout << PC.HI.toString() << PC.LO.toString() << std::endl;
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