// ALU.h - арифметико-логическое устройство
#pragma once
#include <algorithm>
#include "Trit.h"

Tryte Tryte::inc() const {
    Tryte result = *this;
    for (int i = 2; i >= 0; --i) {
        Trit t = result.get(i);
        switch (t) {
        case Trit::Minus:
            result.set(i, Trit::Zero);
            return result;
        case Trit::Zero:
            result.set(i, Trit::Plus);
            return result;
        case Trit::Plus:
            result.set(i, Trit::Minus);
            break; // продолжаем перенос
        }
    }
    return result;
}

Tryte Tryte::dec() const {
    Tryte result = *this;
    for (int i = 2; i >= 0; --i) {
        Trit t = result.get(i);
        switch (t) {
        case Trit::Plus:
            result.set(i, Trit::Zero);
            return result;
        case Trit::Zero:
            result.set(i, Trit::Minus);
            return result;
        case Trit::Minus:
            result.set(i, Trit::Plus);
            break; // продолжаем перенос
        }
    }
    return result;
}

Tryte Tryte::negate() const {
    Tryte result;
    for (int i = 0; i < 3; ++i) {
        Trit t = get(i);
        result.set(i, static_cast<Trit>(-static_cast<int>(t)));
    }
    return result;
}

constexpr Tryte::TritSum Tryte::normalizeTritSum(int sum) {
    if (sum > 1) return { Trit::Minus, +1 };
    if (sum < -1) return { Trit::Plus, -1 };
    return { static_cast<Trit>(sum), 0 };
}

Tryte Tryte::add(const Tryte& rhs) const {
    Tryte result;
    int carry = 0;
    for (int i = 2; i >= 0; --i) {
        int a = static_cast<int>(this->get(i));
        int b = static_cast<int>(rhs.get(i));
        int s = a + b + carry;
        TritSum norm = normalizeTritSum(s);
        result.set(i, norm.value);
        carry = norm.carry;
    }
    return result;
}

Tryte Tryte::sub(const Tryte& rhs) const {
    return this->add(rhs.negate());
}



// Логическое НЕ (по тритам)
Tryte Tryte::logicalNot() const {
    Tryte result;
    for (int i = 0; i < 3; ++i) {
        Trit t = get(i);
        switch (t) {
        case Trit::Plus:  result.set(i, Trit::Minus); break;
        case Trit::Minus: result.set(i, Trit::Plus);  break;
        case Trit::Zero:  result.set(i, Trit::Zero);  break;
        }
    }
    return result;
}

// Троичный AND — поразрядный минимум
Tryte Tryte::logicalAnd(const Tryte& other) const {
    Tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<Trit>(std::min<int>(a, b)));
    }
    return result;
}

// Троичный OR — поразрядный максимум
Tryte Tryte::logicalOr(const Tryte& other) const {
    Tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<Trit>(std::max<int>(a, b)));
    }
    return result;
}

// Троичный XOR — сумма → обрезка до диапазона (−1, 0, +1)
Tryte Tryte::logicalXor(const Tryte& other) const {
    Tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        int x = a + b;
        if (x > 1) x = 1;
        if (x < -1) x = -1;
        result.set(i, static_cast<Trit>(x));
    }
    return result;
}

// Сравнение на равенство (всё ли биты одинаковы)
bool Tryte::equals(const Tryte& other) const {
    return raw() == other.raw();
}

// Сравнение по значению (только если ты задашь порядок тритов!)
bool Tryte::lessThan(const Tryte& other) const {
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        if (a < b) return true;
        if (a > b) return false;
    }
    return false;
}

bool Tryte::greaterThan(const Tryte& other) const {
    return other.lessThan(*this);
}
