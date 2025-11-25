// ALU.h - арифметико-логическое устройство
#pragma once
#include <algorithm>
#include "trit.h"
#include "registers.h"

std::pair<trit, tryte> tryte::inc() const {
    tryte result = *this;
	trit carry = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        trit t = result.get(i);
        switch (t) {
        case trit::Minus:
            result.set(i, trit::Zero);
            return { carry, result };
        case trit::Zero:
            result.set(i, trit::Plus);
            return { carry, result };
        case trit::Plus:
            result.set(i, trit::Minus);
            break; // продолжаем перенос
        }
    }
    carry = trit::Plus;
    return { carry, result };
}

std::pair<trit, tryte> tryte::dec() const {
    tryte result = *this;
    trit carry = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        trit t = result.get(i);
        switch (t) {
        case trit::Plus:
            result.set(i, trit::Zero);
            return  { carry, result };
        case trit::Zero:
            result.set(i, trit::Minus);
            return  { carry, result };
        case trit::Minus:
            result.set(i, trit::Plus);
            break; // продолжаем перенос
        }
    }
    carry = trit::Minus;
    return  { carry, result };
}

/*
// Схема троичного полусумматора

A+ = (A OR trit::Minus) AND (NOT A OR trit::Plus)
A- = (A OR trit::Plus) AND (NOT A OR trit::Minus)
A0 =  NOT(A+ OR A-)

B+ = (B OR trit::Minus) AND (NOT B OR trit::Plus)
B- = (B OR trit::Plus) AND (NOT B OR trit::Minus)
B0 = NOT(B+ OR B-)

C+ = A+ AND B+
C- = A- AND B-
C0 = NOT(C+ OR C-)
C = (C+ AND trit::Plus) OR (C- AND trit::Minus) OR (C0 AND trit::Zero) // перенос

S+ = (A+ AND B0) OR (A0 AND B+) OR (A- AND B-)
S- = (A- AND B0) OR (A0 AND B-) OR (A+ AND B+)
S0 = NOT(S+ OR S-)
S = (S+ AND trit::Plus) OR (S- AND trit::Minus) OR (S0 AND trit::Zero) // сумма


Проблема: если A или B равно 0, то А+, А- или В+, В- тоже будут 0, что является ошибкой. Их результат должен быть -, так A0 или B0 будет равен +.
(C- AND trit::Minus) и (S- AND trit::Minus) можно убрать, так как они не влияют на результат.

Схема троичного сумматора

Входы: A, B, Cin
Выходы: S, Cout
HALFADD — вызов полусумматора

Шаг 1. Складываем A и B
HALFADD A, B -> (S1, C1) 

Шаг 2. Складываем промежуточную сумму S1 и Cin
HALFADD S1, Cin -> (S2, C2)

Шаг 3. Складываем переносы C1 и C2
HALFADD C1, C2 -> (S3, C3)

Шаг 4. Формируем окончательные выходы
S2 -> S // сумма
OR  S3, C3 -> Cout // перенос складываем (чтобы покрыть случай удвоенного переноса)
*/

// Альтернативный сумматор
// А + В + Cin = S + 3*Cout
constexpr tryte::tritSum tryte::normalizetritSum(int sum) {
    switch (sum) {
    case  3: return { trit::Zero,  +1 };
    case  2: return { trit::Minus, +1 };
    case  1: return { trit::Plus,   0 };
    case  0: return { trit::Zero,   0 };
    case -1: return { trit::Minus,  0 };
    case -2: return { trit::Plus,  -1 };
    case -3: return { trit::Zero,  -1 };
    default: std::cerr << "Ошибка в нормализаторе сумм";
    }
}

std::pair<trit, tryte> tryte::add(const tryte& rhs) const {
    tryte result;
    int carry = 0;
    trit carry_flag = trit::Zero; // флаг переполнения
    for (int i = 2; i >= 0; --i) {
        int a = static_cast<int>(this->get(i));
        int b = static_cast<int>(rhs.get(i));
        int s = a + b + carry; // читы

        tritSum norm = normalizetritSum(s);
        result.set(i, norm.value);
        carry = norm.carry;
    }
    if (carry > 0) {
        carry_flag = trit::Plus;   // переполнение в положительную сторону
    }
    else if (carry < 0) {
        carry_flag = trit::Minus;  // переполнение в отрицательную сторону
    }
    return  { carry_flag, result };
}

std::pair<trit, tryte> tryte::sub(const tryte& rhs) const {
    return this->add(rhs.Not());
}



// Логическое НЕ (по тритам)
tryte tryte::Not() const {
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
tryte tryte::And(const tryte& other) const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<trit>(std::min<int>(a, b)));
    }
    return result;
}

// Троичный OR — поразрядный максимум
tryte tryte::Or(const tryte& other) const {
    tryte result;
    for (int i = 0; i < 3; ++i) {
        int a = static_cast<int>(get(i));
        int b = static_cast<int>(other.get(i));
        result.set(i, static_cast<trit>(std::max<int>(a, b)));
    }
    return result;
}

// Троичный XOR — сумма → обрезка до диапазона (−1, 0, +1)
tryte tryte::Xor(const tryte& other) const {
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

// Сравнение на равенство
bool tryte::equals(const tryte& other) const {
    return raw() == other.raw();
}

// Сравнение по значению
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


// Для tword
tword tword::inc() const {
    tword result = *this;

    auto lo_inc = result.LO.inc();
    result.LO = lo_inc.second;

    if (lo_inc.first == trit::Plus) {
        auto hi_inc = result.HI.inc();
        result.HI = hi_inc.second;
    }

    return result;
}

tword tword::dec() const {
    tword result = *this;

    auto lo_dec = result.LO.dec();
    result.LO = lo_dec.second;

    if (lo_dec.first == trit::Minus) {
        auto hi_dec = result.HI.dec();
        result.HI = hi_dec.second;
    }

    return result;
}


trit sign_cmp(const tryte& t) noexcept {
    trit h = t.get(0); // high
    if (h != trit::Zero)
        return h;

    trit m = t.get(1); // mid
    if (m != trit::Zero)
        return m;

    trit l = t.get(2); // low
    return l; // может быть Zero, Plus, Minus
}


tword ar_shf_r(const tword& t) {
	tword tword_result;
    tword_result.HI.set(0, EX);
    tword_result.HI.set(1, t.HI.get(0));
    tword_result.HI.set(2, t.HI.get(1));
    tword_result.LO.set(0, t.HI.get(2));
    tword_result.LO.set(1, t.LO.get(0));
    tword_result.LO.set(2, t.LO.get(1));
	return tword_result;
}

tword ar_shf_l(const tword& t) {
    tword tword_result;
    tword_result.HI.set(0, t.HI.get(1));
    tword_result.HI.set(1, t.HI.get(2));
    tword_result.HI.set(2, t.LO.get(0));
    tword_result.LO.set(0, t.LO.get(1));
    tword_result.LO.set(1, t.LO.get(2));
    tword_result.LO.set(2, trit::Zero);
    return tword_result;
}

tryte tryte::shift_l() const noexcept {
    tryte tryte_result;
    tryte_result.set(0, get(1));
    tryte_result.set(1, get(2));
    tryte_result.set(2, trit::Zero);
    return tryte_result;
}

tryte tryte::shift_r() const noexcept {
    tryte tryte_result;
    tryte_result.set(0, trit::Zero);
    tryte_result.set(1, get(0));
    tryte_result.set(2, get(1));
    return tryte_result;
}



tword mul(tryte& t1, tryte& t2) {
	tword result = (tryte(trit::Zero, trit::Zero, trit::Zero), t2);
    for (int i = 0; i < 3; ++i) {

        if (result.LO.get(2) == trit::Plus) {
            auto [carry, newHI] = result.HI.add(t1);
			EX = carry;
			result.HI = newHI;
        }
        else if (result.LO.get(2) == trit::Minus) {
            auto [carry, newHI] = result.HI.sub(t1);
            EX = carry;
            result.HI = newHI;
        }
		EX = trit::Zero;
    }
	return result;
}

tword div(const tryte& t1, const tryte& t2) {
    tword result = {
        tryte(trit::Zero, trit::Zero, trit::Zero), t1 };

    if (t2 == tryte(trit::Zero, trit::Zero, trit::Zero)) {
        std::cerr << "Ошибка, деление на ноль";
        return result;
    }

    for (int i = 0; i < 3; ++i) {
        result = ar_shf_l(result);

        trit cmp_hi_pos = sign_cmp(result.HI.sub(t2).second);
        trit cmp_hi_neg = sign_cmp(result.HI.sub(t2.Not()).second);

        trit q;

        if (cmp_hi_pos != trit::Minus) {

            auto [carry, newHI] = result.HI.sub(t2);
            result.HI = newHI;
            q = trit::Plus;
        }
        else if (cmp_hi_neg != trit::Plus) {

            auto [carry, newHI] = result.HI.add(t2);
            result.HI = newHI;
            q = trit::Minus;
        }
        else {
            q = trit::Zero;
        }

        result.LO.set(2, q);
    }

    return result;
}