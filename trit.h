// Trit.h Ч определение типа трита и трибайта
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
        default: throw std::invalid_argument("—ука бл€ть, вернул как было и пошел нахуй от сюда");
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

    // „исловое значение в диапазоне [-13, +13]
    /* ѕеренос в TryteUtils
    constexpr int toInt() const {
        int t0 = static_cast<int>(get(0));
        int t1 = static_cast<int>(get(1));
        int t2 = static_cast<int>(get(2));
        return t0 * 9 + t1 * 3 + t2;
    }

    
    static Tryte fromInt(int value) {
        if (value < -13 || value > 13) throw std::out_of_range("Tryte value must be in [-13..+13]");
        int v = value;
        Trit trits[3];
        for (int i = 2; i >= 0; --i) {
            int div = (i == 0) ? 9 : (i == 1) ? 3 : 1;
            int q = v / div;
            v %= div;
            if (q == -2) { trits[2 - i] = Trit::Plus; v -= div; } // балансировка
            else if (q == 2) { trits[2 - i] = Trit::Minus; v += div; }
            else trits[2 - i] = static_cast<Trit>(q);
        }
        return Tryte(trits[0], trits[1], trits[2]);
    }
    */

	// ќтладка, удалить в релизе
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

    constexpr uint8_t raw() const noexcept { return data; }
};