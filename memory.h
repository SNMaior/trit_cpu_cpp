// Memory.hpp Ч определение пам€ти из трайтов
#pragma once
#include <vector>
#include "trit.h"

//  ласс пам€ти: содержит массив Tryte'ов
class Memory {
    std::vector<Tryte> data;

    public:

        //  онструктор: выдел€ет пам€ть под n трайтов
        explicit Memory(size_t size) : data(size) {} // инициализаци€ нул€ми

        // ѕолучение значени€ по адресу
        Tryte get(size_t address) const {
            return data.at(address); // безопасный доступ с проверкой границ
        }

        // «апись значени€ по адресу
        void set(size_t address, const Tryte& value) {
            data.at(address) = value;
        }

        // ѕолучение размера пам€ти в трайтах
        size_t size() const {
            return data.size();
        }
};
