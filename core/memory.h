// memory.hpp — определение памяти из трайтов
#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <variant>
#include "trit.h"
#include "ALU.h"

struct cell {
    std::variant<tryte, pc, trit> value;

    cell() : value(tryte(trit::Zero, trit::Zero, trit::Zero)) {}
    cell(const tryte& tr) : value(tr) {}
    cell(const pc& p) : value(p) {}
    cell(const trit& t) : value(t) {}

    bool isTryte() const noexcept {
        return std::holds_alternative<tryte>(value);
    }

    bool isPc() const noexcept {
        return std::holds_alternative<pc>(value);
    }

    bool isTrit() const noexcept { 
        return std::holds_alternative<trit>(value); 
    }

    tryte asTryte() const {
        return std::get<tryte>(value);
    }

    pc asPc() const {
        return std::get<pc>(value);
    }

    trit asTrit() const {         
        return std::get<trit>(value);     
    }

    std::string toString() const {
        if (isTryte()) return asTryte().toString();
        if (isPc()) return "[" + asPc().HI.toString() + " " + asPc().LO.toString() + "]";
        if (isTrit()) return "Reg(" + std::to_string(static_cast<int>(asTrit())) + ")";
        return "?";
    }
};

// Класс памяти: содержит массив tryte'ов
class Memory {

    std::map<pc, cell> data; // Используем map для динамического выделения памяти

public:

    Memory() = default; // Конструктор по умолчанию

    cell get(const pc& address) const {
        auto it = data.find(address);
        if (it != data.end()) return it->second;
        return cell(); // по умолчанию tryte(0)
    }

    void set(const pc& address, const tryte& value) {
        data[address] = cell(value);
    }

    void set(const pc& address, const pc& value) {
        data[address] = cell(value);
    }

    // Сохранение в текстовом виде (trit::Plus ...)
    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Не удалось открыть файл для записи");

        for (const auto& addr_word : data) {
			const pc& addr = addr_word.first;
			const cell& word = addr_word.second;
            
            out << "[" << addr.HI.toString() << " " << addr.LO.toString() << "] = ";

            if (word.isTryte()) {
                tryte t = word.asTryte();
                out << t.toString();  // если toString уже показывает триты
            }
            else if (word.isPc()) {
                pc p = word.asPc();
                out << "(" << p.HI.toString() << " " << p.LO.toString() << ")";
            }
            out << "\n";
        }
    }

    // Загрузка из текстового вида (trit::Plus ...)
    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Не удалось открыть файл для чтения");

        std::string line;
        pc address = pc{ tryte(trit::Zero, trit::Zero, trit::Zero), tryte(trit::Zero, trit::Zero, trit::Zero) };

        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string a, b, c;

            if (!(iss >> a >> b >> c)) continue;

            trit t0 = stringTotrit(a);
            trit t1 = stringTotrit(b);
            trit t2 = stringTotrit(c);

            data[address] = tryte(t0, t1, t2);

            address = address.inc();
        }
    }

    // Добавление расширения .tcode к имени файла, если его нет
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
        throw std::invalid_argument("Недопустимое значение трита: " + s);
    };
};