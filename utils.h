// Utils.h - разные утилиты
#pragma once
#include "Trit.h"

class Utils {
public:
    static constexpr int toInt(const Tryte& t) {
        int t0 = static_cast<int>(t.get(0));
        int t1 = static_cast<int>(t.get(1));
        int t2 = static_cast<int>(t.get(2));
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
            if (q == -2) { trits[2 - i] = Trit::Plus; v -= div; }
            else if (q == 2) { trits[2 - i] = Trit::Minus; v += div; }
            else trits[2 - i] = static_cast<Trit>(q);
        }
        return Tryte(trits[0], trits[1], trits[2]);
    }
};
