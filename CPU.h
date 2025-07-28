// CPU.h — тритовый процессор с функцией записи числа в память
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "trit.h"
#include "memory.h"
#include "ALU.h"

// Класс CPU: содержит регистры и выполняет инструкции
class CPU {
public:
    bool halted = false;

    Tryte registers[27]; // 27 регистров (3 трита адресуют 3^3)
    size_t pc = 0;           // Счётчик команд
    Memory* memory = nullptr; // Указатель на подключённую память

    // Привязать память к CPU
    void attachMemory(Memory* mem) {
        memory = mem;
    }

    // Преобразование Tryte → int (только от -121 до 121)
    int decodeTryteToInt(const Tryte& word) {
        std::string s = word.toString();
        int result = 0;
        int power = 1;

        for (int i = 2; i >= 0; --i) {
            int digit = 0;
            if (s[i] == '+') digit = 1;
            else if (s[i] == '-') digit = -1;
            result += digit * power;
            power *= 3;
        }

        return result;
    }

    // Преобразование int → Tryte (в пределах [-13, +13])
    Tryte encodeIntToTryte(int value) {
        Trit trits[3] = { Trit::Zero, Trit::Zero, Trit::Zero };

        for (int i = 2; i >= 0; --i) {
            int r = value % 3;
            value /= 3;

            if (r == 2) {
                r = -1;
                value += 1;
            }

            trits[i] = static_cast<Trit>(r);
        }

        return Tryte(trits[0], trits[1], trits[2]);
    }

    // Инструкции
    void executeInstruction(const Tryte& instruction) {
        switch (instruction.raw()) {
        case Tryte(Trit::Plus, Trit::Zero, Trit::Zero).raw(): // +00 HALT
            std::cout << "HALT: выполнение остановлено." << std::endl;
            halted = true;
            break;

        case Tryte(Trit::Plus, Trit::Plus, Trit::Zero).raw(): // ++0 INC R0 ++
            registers[0] = registers[0].inc();
            std::cout << "INC R0 → " << registers[0].toString() << std::endl;
            break;

        case Tryte(Trit::Minus, Trit::Minus, Trit::Zero).raw(): // --0 DEC R0 --
            registers[0] = registers[0].dec();
            std::cout << "DEC R0 → " << registers[0].toString() << std::endl;
            break;

        case Tryte(Trit::Plus, Trit::Zero, Trit::Plus).raw(): {// +0+ LOAD
            Tryte addressWord = memory->get(pc++);
            int addr = decodeTryteToInt(addressWord);
            Tryte value = memory->get(addr);
            registers[0] = value;
            std::cout << "LOAD [" << addr << "] → R0: " << value.toString() << std::endl;
            break;
        }

        case Tryte(Trit::Zero, Trit::Plus, Trit::Plus).raw(): // 0++ ADD R0 + R1 = R0
            std::cout << "ADD R0: " << registers[0].toString() << std::endl;
            std::cout << "ADD R1: " << registers[1].toString() << std::endl;
            registers[0] = registers[0].add(registers[1]);
            std::cout << "ADD R0 + R1 → R0: " << " = " << registers[0].toString() << std::endl;
            break;

        case Tryte(Trit::Zero, Trit::Minus, Trit::Minus).raw(): // 0-- SUB R0 - R1 = R0
            std::cout << "SUB R0: " << registers[0].toString() << std::endl;
            std::cout << "SUB R1: " << registers[1].toString() << std::endl;
            registers[0] = registers[0].sub(registers[1]);
            std::cout << "SUB R0 - R1 → R0: " << " = " << registers[0].toString() << std::endl;
            break;

        case Tryte(Trit::Minus, Trit::Zero, Trit::Plus).raw(): { // -0+ JMP
            Tryte addressWord = memory->get(pc++);
            int addr = decodeTryteToInt(addressWord);
            std::cout << "JMP → " << addr << std::endl;
            pc = addr;
            break;
        }

        case Tryte(Trit::Minus, Trit::Minus, Trit::Minus).raw(): // --- negate
            registers[0] = registers[0].negate();
            break;

        default:
            std::cout << "Неизвестная инструкция: " << instruction.raw() << std::endl;
            break;
        }
    }

    // Основной цикл выполнения
    void run() {
        if (!memory) {
            std::cerr << "Ошибка: память не подключена!" << std::endl;
            return;
        }

        halted = false;
        while (!halted && pc < memory->size()) {
            Tryte instr = memory->get(pc++);
            executeInstruction(instr);
        }
    }
};
