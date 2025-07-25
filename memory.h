// Memory.hpp � ����������� ������ �� �������
#pragma once
#include <vector>
#include "trit.h"

// ����� ������: �������� ������ Tryte'��
class Memory {
    std::vector<Tryte> data;

    public:

        // �����������: �������� ������ ��� n �������
        explicit Memory(size_t size) : data(size) {} // ������������� ������

        // ��������� �������� �� ������
        Tryte get(size_t address) const {
            return data.at(address); // ���������� ������ � ��������� ������
        }

        // ������ �������� �� ������
        void set(size_t address, const Tryte& value) {
            data.at(address) = value;
        }

        // ��������� ������� ������ � �������
        size_t size() const {
            return data.size();
        }
};
