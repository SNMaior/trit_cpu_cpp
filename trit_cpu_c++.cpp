// main.cpp � ������ ��������� ����������
#include <iostream>
#include <windows.h>
#include "trit.h"
#include "memory.h"
#include "CPU.h"
#include "trit6.h"
#include "utils.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "");

	// ������������� ����������
    memory mem(12);
    CPU cpu;
    cpu.attachmemory(&mem);
    
    cpu.registers[0] = tryte(trit::Zero, trit::Plus, trit::Plus);// ������� �� ��������� 4 (0++)
    cpu.registers[1] = tryte(trit::Zero, trit::Plus, trit::Zero); // ������� �� ��������� 3 (0+0)
    cpu.registers[12] = tryte(trit::Plus, trit::Plus, trit::Zero); // ������� �� ��������� 12 (++0)

    // ����� ������� � ������. ������� ������� �������, ����� ������� ��� ��������
    mem.set(0, tryte(trit::Plus, trit::Plus, trit::Zero)); // INC
	mem.set(1, tryte(trit::Zero, trit::Zero, trit::Zero)); // ������� 0, �������� 5 (+--)

    mem.set(2, tryte(trit::Minus, trit::Minus, trit::Zero)); // INC
    mem.set(3, tryte(trit::Zero, trit::Zero, trit::Plus)); // ������� 1, �������� 2 (0+-)

    mem.set(4, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD
	mem.set(5, tryte(trit::Zero, trit::Zero, trit::Zero)); // ������� 0, �������� 7 (+-+)
	mem.set(6, tryte(trit::Zero, trit::Zero, trit::Plus)); // ������� 1

	mem.set(7, tryte(trit::Zero, trit::Minus, trit::Minus)); // SUB
    mem.set(8, tryte(trit::Plus, trit::Plus, trit::Zero)); // ������� 12, �������� 5 (+--)
    mem.set(9, tryte(trit::Zero, trit::Zero, trit::Zero)); // ������� 0

    mem.set(10, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, ������������� ��������� �����������

    // ��������� ���������
    cpu.run();
}