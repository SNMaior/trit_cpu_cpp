// trit.h — определение типа трита и трайта
#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>

enum class trit : int8_t {
    Minus = -1,
    Zero = 0,
    Plus = 1
};

class tryte {
private:
    uint8_t data = 0; // 6 бит на 3 трита (2 бита на каждый)

    static constexpr uint8_t encodetrit(trit t) {
        switch (t) {
        case trit::Minus: return 0b00;
        case trit::Zero:  return 0b01;
        case trit::Plus:  return 0b10;
        }
        return 0xFF; // недостижимый случай
    }

    static constexpr trit decodetrit(uint8_t bits) {
        switch (bits & 0b11) {
        case 0b00: return trit::Minus;
        case 0b01: return trit::Zero;
        case 0b10: return trit::Plus;
        default: throw std::invalid_argument("Не знаю как, но этого быть не должно");
        }
    }

public:
    constexpr tryte() noexcept = default;

    constexpr tryte(trit high, trit mid, trit low) noexcept {
        data = (encodetrit(high) << 4) | (encodetrit(mid) << 2) | encodetrit(low);
    }

    constexpr trit get(int index) const {
        return decodetrit((data >> ((2 - index) * 2)) & 0b11);
    }

    constexpr void set(int index, trit t) {
        uint8_t mask = ~(0b11 << ((2 - index) * 2));
        data = (data & mask) | (encodetrit(t) << ((2 - index) * 2));
    }

    // Отладка, удалить в релизе
    std::string toString() const {
        std::string out;
        for (int i = 0; i < 3; ++i) {
            switch (get(i)) {
            case trit::Minus: out += '-'; break;
            case trit::Zero:  out += '0'; break;
            case trit::Plus:  out += '+'; break;
            }
        }
        return out;
    }

    // Определение оператора сравнения для tryte 
    constexpr bool operator==(const tryte& other) const noexcept {
        return data == other.data;
    }

    constexpr uint8_t raw() const noexcept { return data; }

    std::pair<trit, tryte> inc() const;
    std::pair<trit, tryte> dec() const;

    struct tritSum {
        trit value;
        int carry;
    };

    static constexpr tritSum normalizetritSum(int sum);

    std::pair<trit, tryte> add(const tryte& rhs) const;
    std::pair<trit, tryte> sub(const tryte& rhs) const;

    tryte Not() const;
    tryte And(const tryte& other) const;
    tryte Or(const tryte& other) const;
    tryte Xor(const tryte& other) const;

    bool equals(const tryte& other) const;
    bool lessThan(const tryte& other) const;
    bool greaterThan(const tryte& other) const;

};