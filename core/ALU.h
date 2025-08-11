// ALU.h - арифметико-логическое устройство
#pragma once
#include <algorithm>
#include "trit.h"
#include "registers.h"

std::pair<trit, tryte> tryte::inc() const {
    tryte result = *this;
	EX = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        trit t = result.get(i);
        switch (t) {
        case trit::Minus:
            result.set(i, trit::Zero);
            return { EX, result };
        case trit::Zero:
            result.set(i, trit::Plus);
            return { EX, result };
        case trit::Plus:
            result.set(i, trit::Minus);
            break; // продолжаем перенос
        }
    }
    EX = trit::Plus;
    return { EX, result };
}

std::pair<trit, tryte> tryte::dec() const {
    tryte result = *this;
    EX = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        trit t = result.get(i);
        switch (t) {
        case trit::Plus:
            result.set(i, trit::Zero);
            return  { EX, result };
        case trit::Zero:
            result.set(i, trit::Minus);
            return  { EX, result };
        case trit::Minus:
            result.set(i, trit::Plus);
            break; // продолжаем перенос
        }
    }
    EX = trit::Minus;
    return  { EX, result };
}

tryte tryte::negate() const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        trit t = get(i);
        result.set(i, static_cast<trit>(-static_cast<int>(t)));
    }
    return result;
}

constexpr tryte::tritSum tryte::normalizetritSum(int sum) {
    switch (sum) {
    case  3: return { trit::Zero,  +1 };
    case  2: return { trit::Minus, +1 };
    case  1: return { trit::Plus,   0 };
    case  0: return { trit::Zero,   0 };
    case -1: return { trit::Minus,  0 };
    case -2: return { trit::Plus,  -1 };
    case -3: return { trit::Zero,  -1 };
    }
    return { trit::Zero, 0 }; // на всякий случай
}


std::pair<trit, tryte> tryte::add(const tryte& rhs) const {
    tryte result;
    int carry = 0;
    EX = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        int a = static_cast<int>(this->get(i));
        int b = static_cast<int>(rhs.get(i));
        int s = a + b + carry;

        tritSum norm = normalizetritSum(s);
        result.set(i, norm.value);
        carry = norm.carry;
    }
    if (carry > 0) {
        EX = trit::Plus;   // переполнение в положительную сторону
    }
    else if (carry < 0) {
        EX = trit::Minus;  // переполнение в отрицательную сторону
    }
    return  { EX, result };
}

std::pair<trit, tryte> tryte::sub(const tryte& rhs) const {
    return this->add(rhs.negate());
}



// Логическое НЕ (по тритам)
tryte tryte::logicalNot() const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        trit t = get(i);
        switch (t) {
        case trit::Plus:  result.set(i, trit::Minus); break;
        case trit::Minus: result.set(i, trit::Plus);  break;
        case trit::Zero:  result.set(i, trit::Zero);  break;
        }
    }
    return result;
}

// Троичный AND — поразрядный минимум
tryte tryte::logicalAnd(const tryte& other) const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<trit>(std::min<int>(a, b)));
    }
    return result;
}

// Троичный OR — поразрядный максимум
tryte tryte::logicalOr(const tryte& other) const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<trit>(std::max<int>(a, b)));
    }
    return result;
}

// Троичный XOR — сумма → обрезка до диапазона (−1, 0, +1)
tryte tryte::logicalXor(const tryte& other) const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        int x = a + b;
        if (x > 1) x = 1;
        if (x < -1) x = -1;
        result.set(i, static_cast<trit>(x));
    }
    return result;
}

// Сравнение на равенство (всё ли биты одинаковы)
bool tryte::equals(const tryte& other) const {
    return raw() == other.raw();
}

// Сравнение по значению (только если ты задашь порядок тритов!)
bool tryte::lessThan(const tryte& other) const {
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        if (a < b) return true;
        if (a > b) return false;
    }
    return false;
}

bool tryte::greaterThan(const tryte& other) const {
    return other.lessThan(*this);
}