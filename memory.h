// memory.hpp � ����������� ������ �� �������
#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "trit.h"


// ����� ������: �������� ������ tryte'��
class memory {
    std::vector<tryte> data;

public:

    // �����������: �������� ������ ��� n �������
    explicit memory(size_t size) : data(size) {} // ������������� ������

    // ��������� �������� �� ������
    tryte get(size_t address) const {
        return data.at(address); // ���������� ������ � ��������� ������
    }

    // ������ �������� �� ������
    void set(size_t address, const tryte& value) {
        data.at(address) = value;
    }

    // ��������� ������� ������ � �������
    size_t size() const {
        return data.size();
    }

    // ���������� � ��������� ���� (trit::Plus ...)
    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("�� ������� ������� ���� ��� ������");

        for (const auto& word : data) {
            for (int i = 0; i < 3; ++i) {
                trit t = word.get(i);
                switch (t) {
                case trit::Plus:  out << "trit::Plus";  break;
                case trit::Zero:  out << "trit::Zero";  break;
                case trit::Minus: out << "trit::Minus"; break;
                }
                if (i < 2) out << " ";
            }
            out << "\n";
        }
    }

    // �������� �� ���������� ���� (trit::Plus ...)
    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("�� ������� ������� ���� ��� ������");

        std::string line;
        size_t i = 0;

        while (std::getline(in, line) && i < data.size()) {
            std::istringstream iss(line);
            std::string a, b, c;

            if (!(iss >> a >> b >> c)) continue;

            trit t0 = stringTotrit(a);
            trit t1 = stringTotrit(b);
            trit t2 = stringTotrit(c);

            data[i++] = tryte(t0, t1, t2);
        }
    }

	// ���������� ���������� .tcode � ����� �����, ���� ��� ���
    std::string ensureTcodeExtension(const std::string& filename) {
        if (filename.size() >= 6 && filename.substr(filename.size() - 6) == ".tcode")
            return filename;
        return filename + ".tcode";
    }

private:
    trit stringTotrit(const std::string& s) const {
        if (s == "trit::Plus")  return trit::Plus;
        if (s == "trit::Zero")  return trit::Zero;
        if (s == "trit::Minus") return trit::Minus;
        throw std::invalid_argument("������������ �������� �����: " + s);
    }
};
