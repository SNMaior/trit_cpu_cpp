// Trit.h — определение типа трита и трибайта
#pragma once
#include <string>

enum class Trit : int {
    Minus = -1,
    Zero = 0,
    Plus = 1
};

struct TritWord {
    Trit trits[3];

    TritWord() {
        for (int i = 0; i < 3; ++i) trits[i] = Trit::Zero;
    }

    TritWord(Trit a, Trit b, Trit c) {
        trits[0] = a;
        trits[1] = b;
        trits[2] = c;
    }

    std::string toString() const {
        std::string out = "";
        for (int i = 0; i < 3; ++i) {
            switch (trits[i]) {
            case Trit::Minus: out += '-'; break;
            case Trit::Zero:  out += '0'; break;
            case Trit::Plus:  out += '+'; break;
            }
        }
        return out;
    }
};
