// CPU.h — тритовый процессор с функцией записи числа в память
#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "trit.h"
#include "memory.h"

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
        std::string result = "";

        while (value != 0 && result.length() < 3) {
            int r = value % 3;
            value /= 3;

            if (r == 2) {
                r = -1;
                value += 1;
            }

            if (r == -1) result = "-" + result;
            else if (r == 0) result = "0" + result;
            else if (r == 1) result = "+" + result;
        }

        while (result.length() < 3)
            result = "0" + result;

        return Tryte(
            charToTrit(result[0]),
            charToTrit(result[1]),
            charToTrit(result[2])
        );
    }

    // Инструкции
    void execute(const Tryte& instruction) {
        std::string op = instruction.toString();

        if (op == "+00") {
            std::cout << "HALT: выполнение остановлено." << std::endl;
            halted = true;
        }
        else if (op == "++0") { // INC R0 ++ → R0
            int val = decodeTryteToInt(registers[0]);
            val += 1;
            registers[0] = encodeIntToTryte(val);
            std::cout << "INC R0 → " << val << std::endl;
        }
        else if (op == "--0") { // DEC R0 -- → R0
            int val = decodeTryteToInt(registers[0]);
            val -= 1;
            registers[0] = encodeIntToTryte(val);
            std::cout << "DEC R0 → " << val << std::endl;
        }
        else if (op == "+0+") { // LOAD [адрес] → R0
            Tryte addressWord = memory->get(pc++);
            int addr = decodeTryteToInt(addressWord);
            Tryte value = memory->get(addr);
            registers[0] = value;
            std::cout << "LOAD [" << addr << "] → R0: " << value.toString() << std::endl;
        }
        else if (op == "0++") { // ADD R0 + R1 → R0
            int a = decodeTryteToInt(registers[0]);
            int b = decodeTryteToInt(registers[1]);
            int result = a + b;
            registers[0] = encodeIntToTryte(result);
            std::cout << "ADD R0 + R1 → R0: " << a << " + " << b << " = " << result << std::endl;
        }
        else if (op == "0--") { // SUB R0 - R1 → R0
            int a = decodeTryteToInt(registers[0]);
            int b = decodeTryteToInt(registers[1]);
            int result = a - b;
            registers[0] = encodeIntToTryte(result);
            std::cout << "SUB R0 - R1 → R0: " << a << " - " << b << " = " << result << std::endl;
        }
        else if (op == "-0+") { // JMP
            Tryte addressWord = memory->get(pc++);
            int addr = decodeTryteToInt(addressWord);
            std::cout << "JMP → " << addr << std::endl;
            pc = addr;
        }
        else {
            std::cout << "Неизвестная команда: " << op << std::endl;
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
            execute(instr);
        }
    }


    // Преобразование числа в сбалансированную троичную строку
    std::string toBalancedTernary(int n) {
        if (n == 0) return "0";

        std::string result = "";
        while (n != 0) {
            int r = n % 3;
            n /= 3;

            if (r == 2) {
                r = -1;
                n += 1;
            }

            if (r == -1) result = "-" + result;
            else if (r == 0) result = "0" + result;
            else if (r == 1) result = "+" + result;
        }

        // Подгонка под 6 тритов (2 трибайта)
        while (result.length() < 6)
            result = "0" + result;

        return result;
    }

    // Преобразование символа в Trit
    Trit charToTrit(char c) {
        switch (c) {
        case '+': return Trit::Plus;
        case '0': return Trit::Zero;
        case '-': return Trit::Minus;
        default:
            throw std::invalid_argument("Недопустимый символ в трит-строке");
        }
    }

    // Сохранить целое число в память как 2 трибайта (6 тритов)
    void storeIntegerToMemory(int value, size_t address) {
        if (!memory) {
            std::cerr << "Ошибка: память не подключена!" << std::endl;
            return;
        }

        std::string ternary = toBalancedTernary(value);
        std::cout << "Симметричная троичная запись: " << ternary << std::endl;

        for (int i = 0; i < 2; ++i) {
            Trit a = charToTrit(ternary[i * 3]);
            Trit b = charToTrit(ternary[i * 3 + 1]);
            Trit c = charToTrit(ternary[i * 3 + 2]);
            Tryte word(a, b, c);
            memory->set(address + i, word);

            std::cout << "Ячейка памяти " << (address + i) << ": " << word.toString() << std::endl;
        }
    }
};
