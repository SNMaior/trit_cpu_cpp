// Utils.h - разные утилиты
#pragma once
#include "Trit.h"

class Utils {
public:
    static int toInt(const Tryte& t) {
        int t0 = static_cast<int>(t.get(0));
        int t1 = static_cast<int>(t.get(1));
        int t2 = static_cast<int>(t.get(2));
        return t0 * 9 + t1 * 3 + t2;
    }

    static Tryte fromInt(int value) {
        Trit trits[3] = { Trit::Zero, Trit::Zero, Trit::Zero };
        for (int i = 2; i >= 0; --i) {
            int r = value % 3;
            value /= 3;
            if (r == 2) {
                r = -1;
                value += 1;
            }
            trits[i] = static_cast<Trit>(r);
        }
        return Tryte(trits[0], trits[1], trits[2]);
    }
};
