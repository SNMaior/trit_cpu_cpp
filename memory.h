// Memory.hpp � ����������� ������ �� ���������
#pragma once
#include <vector>
#include "trit.h"

// ����� ������: �������� ������ TritWord'��
class Memory {
    public:
        std::vector<TritWord> data;

        // �����������: �������� ������ ��� n ���������
        Memory(size_t size) {
            data.resize(size); // ������������� ������
        }

        // ��������� �������� �� ������
        TritWord get(size_t address) const {
            return data.at(address); // ���������� ������ � ��������� ������
        }

        // ������ �������� �� ������
        void set(size_t address, const TritWord& word) {
            data.at(address) = word;
        }

        // ��������� ������� ������ � ���������
        size_t size() const {
            return data.size();
        }
};
