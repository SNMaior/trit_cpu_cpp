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

    // ����� ������� � ������. ������� ������� �������, ����� ������� ��� ��������
    mem.set(0, tryte(trit::Plus, trit::Plus, trit::Zero)); // INC, �������� 5 (+--)
    mem.set(1, tryte(trit::Plus, trit::Zero, trit::Plus)); // LOAD
    mem.set(2, utils::fromInt(5)); // �������, ��� ������ �������� ���������� �� 5 ����
	mem.set(3, tryte(trit::Minus, trit::Zero, trit::Plus)); // JMP
	mem.set(4, utils::fromInt(6)); // �������, ��� ������ ������� �� 6 ���
    mem.set(5, utils::fromInt(10)); // �������� ��� LOAD, ������� ������� � R0, � ������ ������ ��� 10 (+0+)
    mem.set(6, tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD, �������� 13 (+++) 
    mem.set(7, tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, ������������� ��������� �����������

    // ��������� ���������
    cpu.run();
    tryte HALT = mem.get(5);
    std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // ������� R0 ��� �������
    std::cout << "��� 5 = " << HALT.toString() << std::endl; // ������� ��� 5 ��� �������
}

