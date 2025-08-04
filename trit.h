// trit.h � ����������� ���� ����� � ������
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
    uint8_t data = 0; // 6 ��� �� 3 ����� (2 ���� �� ������)

    static constexpr uint8_t encodetrit(trit t) {
        switch (t) {
        case trit::Minus: return 0b00;
        case trit::Zero:  return 0b01;
        case trit::Plus:  return 0b10;
        }
        return 0xFF; // ������������ ������
    }

    static constexpr trit decodetrit(uint8_t bits) {
        switch (bits & 0b11) {
        case 0b00: return trit::Minus;
        case 0b01: return trit::Zero;
        case 0b10: return trit::Plus;
        default: throw std::invalid_argument("�� ���� ���, �� ����� ���� �� ������");
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

    // �������, ������� � ������
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

    // ����������� ��������� ��������� ��� tryte 
    constexpr bool operator==(const tryte& other) const noexcept {
        return data == other.data;
    }

    constexpr uint8_t raw() const noexcept { return data; }

    tryte inc() const;
    tryte dec() const;
    tryte negate() const;

    struct tritSum {
        trit value;
        int carry;
    };

    static constexpr tritSum normalizetritSum(int sum);

    tryte add(const tryte& rhs) const;
    tryte sub(const tryte& rhs) const;

    tryte logicalNot() const;
    tryte logicalAnd(const tryte& other) const;
    tryte logicalOr(const tryte& other) const;
    tryte logicalXor(const tryte& other) const;

    bool equals(const tryte& other) const;
    bool lessThan(const tryte& other) const;
    bool greaterThan(const tryte& other) const;

};