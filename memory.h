// Memory.hpp � ����������� ������ �� �������
#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
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

    // ���������� � ��������� ���� (Trit::Plus ...)
    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("�� ������� ������� ���� ��� ������");

        for (const auto& word : data) {
            for (int i = 0; i < 3; ++i) {
                Trit t = word.get(i);
                switch (t) {
                case Trit::Plus:  out << "Trit::Plus";  break;
                case Trit::Zero:  out << "Trit::Zero";  break;
                case Trit::Minus: out << "Trit::Minus"; break;
                }
                if (i < 2) out << " ";
            }
            out << "\n";
        }
    }

    // �������� �� ���������� ���� (Trit::Plus ...)
    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("�� ������� ������� ���� ��� ������");

        std::string line;
        size_t i = 0;

        while (std::getline(in, line) && i < data.size()) {
            std::istringstream iss(line);
            std::string a, b, c;

            if (!(iss >> a >> b >> c)) continue;

            Trit t0 = stringToTrit(a);
            Trit t1 = stringToTrit(b);
            Trit t2 = stringToTrit(c);

            data[i++] = Tryte(t0, t1, t2);
        }
    }

	// ���������� ���������� .tcode � ����� �����, ���� ��� ���
    std::string ensureTcodeExtension(const std::string& filename) {
        if (filename.size() >= 6 && filename.substr(filename.size() - 6) == ".tcode")
            return filename;
        return filename + ".tcode";
    }

private:
    Trit stringToTrit(const std::string& s) const {
        if (s == "Trit::Plus")  return Trit::Plus;
        if (s == "Trit::Zero")  return Trit::Zero;
        if (s == "Trit::Minus") return Trit::Minus;
        throw std::invalid_argument("������������ �������� �����: " + s);
    }
};
