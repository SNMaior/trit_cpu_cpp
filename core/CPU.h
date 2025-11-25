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

    Memory* memory_cpu = nullptr; // Указатель на подключённую память

    // Привязать память к CPU
    void attachmemory(Memory* mem) {
        memory_cpu = mem;
    }

    pc PC;
    rsp RSP = {tryte(trit::Plus, trit::Plus, trit::Plus), tryte(trit::Plus, trit::Plus, trit::Plus)};
    Wide_Reg wreg;
    Registers& registers;
    CPU(Registers& regs) : registers(regs) {}

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
            auto [carry, result] = registers[reg].inc();
            EX = carry;
            registers[reg] = result;
            std::cout << "INC reg > "<< static_cast<int>(EX) << registers[reg].toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Zero).raw(): { // --0 DEC R0 --
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            auto [carry, result] = registers[reg].dec();
            EX = carry;
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
            auto [carry, result] = registers[regA].add(registers[regB]);
            EX = carry;
            registers[regA] = result;
            std::cout << "ADD R0 + R1 > R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Minus).raw(): { // 0-- SUB R0 - R1 = R0
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "SUB regA > " << registers[regA].toString() << std::endl;
            std::cout << "SUB regB > " << registers[regB].toString() << std::endl;
            auto [carry, result] = registers[regA].sub(registers[regB]);
            EX = carry;
            registers[regA] = result;
            std::cout << "SUB R0 - R1 > R0: " << " = " << static_cast<int>(EX) << registers[regA].toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Plus).raw(): { // 0-+ JMP
            pc addr = memory_cpu->get(PC.inc()).asPc();
			PC = addr;
            std::cout << "JMP > " << addr.HI.toString() << addr.LO.toString() << std::endl;
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Minus).raw(): { // --- NOT
            tryte reg = memory_cpu->get(PC.inc()).asTryte();
            registers[reg] = registers[reg].Not();
            break;
        }

        case tryte(trit::Zero, trit::Plus, trit::Zero).raw(): { // 0+0 MOV
            trit mode = memory_cpu->get(PC.inc()).asTrit();
            tryte reg_dst = memory_cpu->get(PC.inc()).asTryte();
            if (mode == trit::Minus) { // число
                registers[reg_dst] = memory_cpu->get(PC.inc()).asTryte();
                std::cout << "MOV R" << reg_dst.toString() << " == " << registers[reg_dst].toString() << std::endl;
            }
            else if (mode == trit::Plus) { // регистры
                wreg[reg_dst] = registers[memory_cpu->get(PC.inc()).asTryte()];
            }
            else if (mode == trit::Zero) { // EX+tryte
                wreg[reg_dst] = tword{ tryte(trit::Zero, trit::Zero, EX), registers[memory_cpu->get(PC.inc()).asTryte()] };
				EX = trit::Zero;
			}
            else {
                std::cerr << "Ошибка: неверный режим!" << std::endl;
            }
            break;
        }

        case tryte(trit::Zero, trit::Minus, trit::Zero).raw(): { // 0-0 CMP R0 - R1 = logic
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "CMP regA > " << registers[regA].toString() << std::endl;
            std::cout << "CMP regB > " << registers[regB].toString() << std::endl;
            auto [carry, result] = registers[regA].sub(registers[regB]);

            // Если carry != Zero — был выход за диапазон, и знак нам даёт carry
            if (carry != trit::Zero) {
                LG = carry;           // переполнение определяет знак сравнения
            }
            else {
                LG = sign_cmp(result);  // иначе берем знак результата
            }

            std::cout << "CMP R0 == R1" << std::endl;
            std::cout << "LG:" << " = " << static_cast<int>(LG) << std::endl;
            break;
        }

        case tryte(trit::Plus, trit::Minus, trit::Plus).raw(): { // +-+ JE  
            pc addr = memory_cpu->get(PC.inc()).asPc();
            if (LG == trit::Zero) {
                PC = addr;
                std::cout << "JE выполнено, переход в " << addr.HI.toString() << addr.LO.toString() << std::endl;
            }
            else {
                std::cout << "JE не выполнено" << std::endl;
            }
            break;
        }

        case tryte(trit::Minus, trit::Minus, trit::Plus).raw(): { // --+ JNE  
            pc addr = memory_cpu->get(PC.inc()).asPc();
            if (LG != trit::Zero) {
				PC = addr;
                std::cout << "JNE выполнено, переход в " << addr.HI.toString() << addr.LO.toString() << std::endl;
            }
            else {
                std::cout << "JNE не выполнено" << std::endl;
            }
            break;
        }

        case tryte(trit::Minus, trit::Plus, trit::Plus).raw(): { // -++ PUSH
			tryte reg_src = memory_cpu->get(PC.inc()).asTryte(); // откуда берём данные
            rsp RSP_MAX = { tryte(trit::Plus, trit::Plus, trit::Plus), tryte(trit::Minus, trit::Minus, trit::Minus) };
            if (RSP == RSP_MAX || SP == trit::Plus) {
                SP = trit::Plus;
                std::cerr << "Ошибка: переполнение стека!" << std::endl;
            }
            else {
                memory_cpu->set(RSP, registers[reg_src]);
                RSP.dec();
                SP = trit::Zero;
                std::cout << "PUSH R" << reg_src.toString() << " > [" << RSP.HI.toString() << RSP.LO.toString() << "]: " << registers[reg_src].toString() << std::endl;
			}
            break;
        }

        case tryte(trit::Minus, trit::Plus, trit::Minus).raw(): { // -+- POP
            tryte reg_dst = memory_cpu->get(PC.inc()).asTryte(); // куда грузим
            rsp RSP_MIN = { tryte(trit::Plus, trit::Plus, trit::Plus), tryte(trit::Plus, trit::Plus, trit::Plus) };
            if (RSP == RSP_MIN || SP == trit::Minus) {
                SP = trit::Minus;
                std::cerr << "Ошибка: стек пуст!" << std::endl;
            }
            else {
                RSP.inc();
                registers[reg_dst] = memory_cpu->get(RSP).asTryte();
                SP = trit::Zero;
                std::cout << "POP R" << reg_dst.toString() << " > [" << RSP.HI.toString() << RSP.LO.toString() << "]: " << registers[reg_dst].toString() << std::endl;
            }
            break;
        }


        case tryte(trit::Plus, trit::Zero, trit::Zero).raw(): { // +00 CALL
			pc addr = memory_cpu->get(PC.inc()).asPc(); // куда прыгаем
			pc addr_ret = PC.inc(); // адрес возврата
            rsp RSP_MAX = { tryte(trit::Plus, trit::Plus, trit::Plus), tryte(trit::Minus, trit::Minus, trit::Minus) };
            if (RSP == RSP_MAX || SP == trit::Plus) {
                SP = trit::Plus;
                std::cerr << "Ошибка: переполнение стека!" << std::endl;
            }
            else {
                PC = addr;
                memory_cpu->set(RSP, addr_ret);
                RSP.dec();
                SP = trit::Zero;
            }
            break;
        }
        
        case tryte(trit::Minus, trit::Zero, trit::Zero).raw(): { // -00 RET
            rsp RSP_MIN = { tryte(trit::Plus, trit::Plus, trit::Plus), tryte(trit::Plus, trit::Plus, trit::Plus) };
            if (RSP == RSP_MIN || SP == trit::Minus) {
                SP = trit::Minus;
                std::cerr << "Ошибка: стек пуст!" << std::endl;
            }
            else {
                RSP.inc();
                PC = memory_cpu->get(RSP).asPc();
                SP = trit::Zero;
            }
            break;
        }

        case tryte(trit::Plus, trit::Plus, trit::Plus).raw(): { // +++ SHF
            tryte reg_src = memory_cpu->get(PC.inc()).asTryte();
			trit mode = memory_cpu->get(PC.inc()).asTrit();

            if (mode == trit::Minus){ // вправо
                registers[reg_src] = registers[reg_src].shift_r();
			}
            else if (mode == trit::Plus) { // влево
                registers[reg_src] = registers[reg_src].shift_l();
            }
            else {
                std::cerr << "Ошибка: неверный режим сдвига!" << std::endl;
			}
            break;
        }

        case tryte(trit::Zero, trit::Zero, trit::Minus).raw(): { // 00+ ASH
            tryte reg_src = memory_cpu->get(PC.inc()).asTryte();
            trit mode = memory_cpu->get(PC.inc()).asTrit();

            if (mode == trit::Minus) { // вправо
                wreg[tryte(trit::Plus, trit::Plus, trit::Zero)] = ar_shf_r(registers[reg_src]);
            }
            else if (mode == trit::Plus) { // влево
                wreg[tryte(trit::Plus, trit::Plus, trit::Zero)] = ar_shf_l(registers[reg_src]);
            }
            else {
                std::cerr << "Ошибка: неверный режим сдвига!" << std::endl;
            }
            break;
        }


        case tryte(trit::Minus, trit::Plus, trit::Zero).raw(): { // -+0 MUL
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "MUL regA > " << registers[regA].toString() << std::endl;
            std::cout << "MUL regB > " << registers[regB].toString() << std::endl;
			wreg[tryte(trit::Plus, trit::Plus, trit::Plus)] = mul(registers[regA], registers[regB]);
            std::cout << "MUL WR0 = " << wreg[tryte(trit::Plus, trit::Plus, trit::Plus)].HI.toString() << wreg[tryte(trit::Plus, trit::Plus, trit::Plus)].LO.toString() << std::endl;
            break;
        }

        case tryte(trit::Zero, trit::Plus, trit::Minus).raw(): { // 0+- DIV
            tryte regA = memory_cpu->get(PC.inc()).asTryte();
            tryte regB = memory_cpu->get(PC.inc()).asTryte();
            std::cout << "DIV regA > " << registers[regA].toString() << std::endl;
            std::cout << "DIV regB > " << registers[regB].toString() << std::endl;
            wreg[tryte(trit::Plus, trit::Plus, trit::Plus)] = div(registers[regA], registers[regB]);
            std::cout << "DIV WR0 = " << wreg[tryte(trit::Plus, trit::Plus, trit::Plus)].HI.toString() << wreg[tryte(trit::Plus, trit::Plus, trit::Plus)].LO.toString() << std::endl;
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
| 00+ | HLT        | Остановка программы              000 |
| ??? | RESET      | Сброс процессора                 00+ |
| +0+ | LOAD       | Загрузка значения в регистр          |
| +0- | STORE      | Запись значения регистра в память    |
| -0+ | LOADM      | Загрузка из памяти в регистр         |
| -0- | STOREM     | Запись в память из регистра          |
| ++0 | INC        | Увеличение регистра на 1             |
| --0 | DEC        | Уменьшение регистра на 1             |
| 0++ | ADD        | Сложение                             |
| 0-- | SUB        | Вычитание                            |
| 0-+ | JMP        | Безусловный переход                  |
| +-+ | JE         | Переход если равно                   |
| --+ | JNE        | Переход если не равно                |
| 0-0 | CMP        | Сравнение                            |
| -++ | PUSH       | Поместить в стек                     |
| -+- | POP        | Извлечь из стека                     |
| +00 | CALL       | Вызов подпрограммы                   |
| -00 | RET        | Возврат из подпрограммы              |
| 0+0 | MOV        | Копировать значение между регистрами |
| -+0 | MUL        | Умножение                            |
| 0+- | DIV        | Деление                              |
| +-0 | AND        | Побитовое И                          |
| +-- | OR         | Побитовое ИЛИ                        |
| ++- | XOR        | Исключающее ИЛИ                      |
| --- | NOT        | Побитовое отрицание                  |
| +++ | SHF        | Сдвиг трита                          |
| 00- | ASH        | Арифметический сдвиг трита           |

*/