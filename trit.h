// Trit.h — определение типа трита и трайта
#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>

enum class Trit : int8_t {
    Minus = -1,
    Zero = 0,
    Plus = 1
};

class Tryte {
private:
    uint8_t data = 0; // 6 бит на 3 трита (2 бита на каждый)

    static constexpr uint8_t encodeTrit(Trit t) {
        switch (t) {
        case Trit::Minus: return 0b00;
        case Trit::Zero:  return 0b01;
        case Trit::Plus:  return 0b10;
        }
        return 0xFF; // недостижимый случай
    }

    static constexpr Trit decodeTrit(uint8_t bits) {
        switch (bits & 0b11) {
        case 0b00: return Trit::Minus;
        case 0b01: return Trit::Zero;
        case 0b10: return Trit::Plus;
        default: throw std::invalid_argument("Не знаю как, но этого быть не должно");
        }
    }

public:
    constexpr Tryte() noexcept = default;

    constexpr Tryte(Trit high, Trit mid, Trit low) noexcept {
        data = (encodeTrit(high) << 4) | (encodeTrit(mid) << 2) | encodeTrit(low);
    }

    constexpr Trit get(int index) const {
        return decodeTrit((data >> ((2 - index) * 2)) & 0b11);
    }

    constexpr void set(int index, Trit t) {
        uint8_t mask = ~(0b11 << ((2 - index) * 2));
        data = (data & mask) | (encodeTrit(t) << ((2 - index) * 2));
    }

	// Отладка, удалить в релизе
    std::string toString() const {
        std::string out;
        for (int i = 0; i < 3; ++i) {
            switch (get(i)) {
            case Trit::Minus: out += '-'; break;
            case Trit::Zero:  out += '0'; break;
            case Trit::Plus:  out += '+'; break;
            }
        }
        return out;
    }

	// Определение оператора сравнения для Tryte 
    constexpr bool operator==(const Tryte& other) const noexcept {
        return data == other.data;
    }

    constexpr uint8_t raw() const noexcept { return data; }

    Tryte inc() const;
    Tryte dec() const;
    Tryte negate() const;

    struct TritSum {
        Trit value;
        int carry;
    };

    static constexpr TritSum normalizeTritSum(int sum);

    Tryte add(const Tryte& rhs) const;
    Tryte sub(const Tryte& rhs) const;

    Tryte logicalNot() const;
    Tryte logicalAnd(const Tryte& other) const;
    Tryte logicalOr(const Tryte& other) const;
    Tryte logicalXor(const Tryte& other) const;

    bool equals(const Tryte& other) const;
    bool lessThan(const Tryte& other) const;
    bool greaterThan(const Tryte& other) const;

};