// Memory.hpp Ч определение пам€ти из трибайтов
#pragma once
#include <vector>
#include "trit.h"

//  ласс пам€ти: содержит массив TritWord'ов
class Memory {
    public:
        std::vector<TritWord> data;

        //  онструктор: выдел€ет пам€ть под n трибайтов
        Memory(size_t size) {
            data.resize(size); // инициализаци€ нул€ми
        }

        // ѕолучение значени€ по адресу
        TritWord get(size_t address) const {
            return data.at(address); // безопасный доступ с проверкой границ
        }

        // «апись значени€ по адресу
        void set(size_t address, const TritWord& word) {
            data.at(address) = word;
        }

        // ѕолучение размера пам€ти в трибайтах
        size_t size() const {
            return data.size();
        }
};
