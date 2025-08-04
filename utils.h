// utils.h - разные утилиты
#pragma once
#include "trit.h"

class utils {
public:
    static int toInt(const tryte& t) {
        int t0 = static_cast<int>(t.get(0));
        int t1 = static_cast<int>(t.get(1));
        int t2 = static_cast<int>(t.get(2));
        return t0 * 9 + t1 * 3 + t2;
    }

    static tryte fromInt(int value) {
        trit trits[3] = { trit::Zero, trit::Zero, trit::Zero };
        for (int i = 2; i >= 0; --i) {
            int r = value % 3;
            value /= 3;
            if (r == 2) {
                r = -1;
                value += 1;
            }
            trits[i] = static_cast<trit>(r);
        }
        return tryte(trits[0], trits[1], trits[2]);
    }
};