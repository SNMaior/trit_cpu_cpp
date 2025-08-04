# trit_cpu_c++
�������� ��������� �� C++. ������� ��� ������������ �����, ������� ����� ��������� �������� -1, 0 � 1. � ������ ������� ��� ����������� � ���� ������ "trit" � ��������� ��� "trit::Minus", "trit::Zero" � "trit::Plus".

���������� �����, ����� ����� ���� ���������� � ������ �� 3 �����, ������� ����������� � ���� ������ "Tryte" (�����). ����� ����� ��������� �������� �� -13 �� 13.


� ALU.h ����������� �������� �������������� �������� ��� ������� � ��������, � ����� ���������� �������� ��� �������. �� � ���� �� ���������������� ��������. ������ ����������� �������� inc(), dec(), negate(), add(), sub(), � ����� ���������� �������� logicalNot(), logicalAnd(), logicalOr(), logicalXor(), equals(), lessThan().

���������������� ���� ������ inc(), dec(), negate(), add(), sub().


memory.h ��������� ������ ����������. ������ ����� ���� ���������������� "memory mem(int);" ��� "memory memory(int);". 

"memory mem(int);" �������� �������� RAM. ����� "memory mem(int);" ���������� ��������� ������ � ���������� � ������� "cpu.attachmemory(&mem);". 

"memory memory(int);" �������� �������� ROM. ����� "memory memory(int);" ����� �������, �������� "memory.set(0, Tryte(trit::Plus, trit::Plus, trit::Zero));", � � ����� ��������� � ����� ���� ����� ��������� "memory.saveToFile("���_�����.tcode");". ��� �������� ������ �� ����� ���������� "mem.loadFromFile("���_�����.tcode");".

� ����� ������� int ���������� ���������� ����� ������. 1 ������ = 1 �������.


CPU.h ��� �������� ��������� ������ ����� ����� ������������ ����������. � ��� ����������� �������, ������� �� ������ ������������ � ����� ����. ��� ��� ������ ��������� ������������� ������ "memory mem(int);", ��������� "CPU cpu;", ��������� ������ � ���������� "cpu.attachmemory(&mem);" � ��������� "cpu.run();".

����� CPU.h ������������ 14 ���������, ������� ����� ������������ � ����. ��� ������������ ��� registers[0], registers[1], ..., registers[13].


����������, ������� ����� ������������ � ����:

HALT - ��������� ����������, ���������� "trit::Plus, trit::Zero, trit::Zero" !!!������ ����������� � ����� ����� ���������!!!

INC - ��������� ��������, ���������� "trit::Plus, trit::Plus, trit::Zero"

DEC - ��������� ��������, ���������� "trit::Minus, trit::Minus, trit::Zero"

LOAD - �������� �������� �� ������ � �������, ���������� "trit::Plus, trit::Zero, trit::Plus"

ADD - �������� ���� ���������, ���������� "trit::Zero, trit::Plus, trit::Plus"

SUB - ��������� ���� ���������, ���������� "trit::Zero, trit::Minus, trit::Minus"

JMP - ������� � �����, ���������� "trit::Minus, trit::Zero, trit::Plus"

negate - ������ ������� ���������������, ���������� "trit::Minus, trit::Minus, trit::Minus"


HALT ���������� �� �����. INC, DEC � negate �������� � ����� ����������, ��������� ������ ���������� � ������ ���������. ADD � SUB �������� � ����� �����������, ��������� ������ ���������� � ������ ���������. � LOAD ��������� ����� ������ ���� ��������, ������� ��������� �� ����� ��� �������, � ������� ������ ���� �������� ��� ��������. � JMP ��������� ����� ������ ���� ��������, ������� ��������� �� ����� ��� �������.


toString() ������� ��������� ������������� ������ ������ ��� �������.


Utils.h �������� ������� ��� ������ �������������� Tryte To Int � ��������. 


������ �������������:
```cpp
// ������������� ����������
memory mem(12);
CPU cpu;
cpu.attachmemory(&mem);

cpu.registers[0] = Tryte(trit::Zero, trit::Plus, trit::Plus);// ������� �� ��������� 4 (0++)
cpu.registers[1] = Tryte(trit::Zero, trit::Plus, trit::Zero); // ������� �� ��������� 3 (0+0)

// ����� ������� � ������. ������� ������� �������, ����� ������� ��� ��������
mem.set(0, Tryte(trit::Plus, trit::Plus, trit::Zero)); // INC, �������� 5 (+--)
mem.set(1, Tryte(trit::Plus, trit::Zero, trit::Plus)); // LOAD
mem.set(2, Utils::fromInt(5)); // �������, ��� ������ �������� ���������� �� 5 ����
mem.set(3, Tryte(trit::Minus, trit::Zero, trit::Plus)); // JMP
mem.set(4, Utils::fromInt(6)); // �������, ��� ������ ������� �� 6 ���
mem.set(5, Utils::fromInt(10)); // �������� ��� LOAD, ������� ������� � R0, � ������ ������ ��� 10 (+0+)
mem.set(6, Tryte(trit::Zero, trit::Plus, trit::Plus)); // ADD, �������� 13 (+++) 
mem.set(7, Tryte(trit::Plus, trit::Zero, trit::Zero)); // HALT, ������������� ��������� �����������

// ��������� ���������
cpu.run();

std::cout << "R0 = " << cpu.registers[0].toString() << std::endl; // ������� R0 ��� �������
```


!!!�����!!!

HALT ������������ � ����� ��������� �����������.

����� LOAD �� ������ mem.set(1, Utils::fromInt(5)); mem.set(2, Utils::fromInt(10));, ������� �� ������, ��� �� ����� ��������. ����������� ����������� JMP ��� � ������� ���
```cpp
mem.set(0, Tryte(trit::Plus, trit::Zero, trit::Plus));
mem.set(1, Utils::fromInt(3));
mem.set(2, Tryte(trit::Plus, trit::Plus, trit::Zero));
mem.set(3, Utils::fromInt(10));
mem.set(4, Tryte(trit::Plus, trit::Zero, trit::Zero));
```
