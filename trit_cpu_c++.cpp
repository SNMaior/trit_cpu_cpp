/*#include <iostream>
#include <string>
#include <locale>
#include <windows.h>
#include <cmath>
#include <vector>
#include <utility>
using namespace std;

// Перевод из десятичного в симметричную троичную систему
string toBalancedTernary(long long n) {
    if (n == 0) return "0";
    string result = "";

    while (n != 0) {
        int remainder = n % 3;

        // Обработка "переполнения": 2 => -1 + 1 перенос
        if (remainder == 2) {
            remainder = -1;
            n += 1;
        }
        else if (remainder == -2) {
            remainder = 1;
            n -= 1;
        }

        // Преобразуем остаток в символ
        if (remainder == -1) result = "-" + result;
        else if (remainder == 0) result = "0" + result;
        else if (remainder == 1) result = "+" + result;

        n /= 3;
    }

    return result;
}

// Обратный перевод: из симметричной троичной в десятичную
long long fromBalancedTernary(const string& s) {
    long long result = 0;
    int power = 0;

    // Читаем строку справа налево
    for (int i = s.length() - 1; i >= 0; --i) {
        char c = s[i];
        int digit = 0;

        // Преобразуем символ в значение
        if (c == '+') digit = 1;
        else if (c == '-') digit = -1;
        else if (c == '0') digit = 0;
        else {
            cout << "Ошибка: недопустимый символ '" << c << "' в строке." << endl;
            return 0;
        }

        result += digit * pow(3, power);
        power++;
    }

    return result;
}

// Структура для хранения результата кодирования
struct Encoded {
    long long mantissa;
    string scaleStr;
};

// Таблица масштабов: строка симметричной троичной -> степень десяти
vector<pair<string, double>> scaleTable = {
    {"---", 1e-13}, {"--0", 1e-12}, {"--+", 1e-11},
    {"-0-", 1e-10}, {"-00", 1e-9},  {"-0+", 1e-8},
    {"-+-", 1e-7},  {"-+0", 1e-6},  {"-++", 1e-5},
    {"0--", 1e-4},  {"0-0", 1e-3},  {"0-+", 1e-2},
    {"00-", 1e-1},  {"000", 1.0},   {"00+", 1e1},
    {"0+0", 1e2},   {"+--", 1e3},   {"+-0", 1e4},
    {"+-+", 1e5},   {"+0-", 1e6},   {"+00", 1e7},
    {"+0+", 1e8},   {"++-", 1e9},   {"++0", 1e10},
    {"+++", 1e11}
};

// Функция кодирования: находит подходящую масштабную строку и мантиссу
Encoded encode(double x) {
    for (const auto& entry : scaleTable) {
        const string& scaleStr = entry.first;
        double scaleVal = entry.second;

        double mant = x / scaleVal;

        if (fabs(mant - round(mant)) < 1e-9) {
            return { static_cast<long long>(round(mant)), scaleStr };
        }
    }

    // Если подходящая степень не найдена — возвращаем по умолчанию
    return { 0, "000" };
}

int main() {
    // Настройка консоли под UTF-8 и локаль
    SetConsoleOutputCP(65001);   // Вывод в UTF-8
    SetConsoleCP(65001);         // Ввод в UTF-8
    setlocale(LC_ALL, "");       // Локаль по умолчанию

    // Меню
    cout << "=== Симметричный троичный калькулятор ===" << endl;
    cout << "1. Перевести из десятичного в симметричную троичную" << endl;
    cout << "2. Перевести из симметричной троичной в десятичную" << endl;
    cout << "3. Кодировать число как mantissa × 10^scaleStr" << endl;
    cout << "Выберите режим (1, 2 или 3): ";

    int mode;
    cin >> mode;

    if (mode == 1) {
        // Перевод из десятичного в симметричную троичную
        long long n;
        cout << "Введите десятичное число: ";
        cin >> n;
        string result = toBalancedTernary(n);
        cout << "Симметричное троичное представление: " << result << endl;
    }
    else if (mode == 2) {
        // Перевод обратно в десятичную
        string ternary;
        cout << "Введите симметричное троичное число: ";
        cin >> ternary;
        long long result = fromBalancedTernary(ternary);
        cout << "Десятичное значение: " << result << endl;
    }
    else if (mode == 3) {
        // Кодирование дробного числа
        double value;
        cout << "Введите число для кодирования (например 123.0): ";
        cin >> value;
        Encoded encoded = encode(value);
        string btMantissa = toBalancedTernary(encoded.mantissa);
        cout << "Кодировка: " << btMantissa << " × 10^" << encoded.scaleStr << endl;
    }
    else {
        cout << "Неверный выбор. Завершение программы." << endl;
    }

    return 0;
}
*/

// main.cpp — запуск тритового процессора
#include <iostream>
#include <windows.h>
#include "trit.h"
#include "memory.h"
#include "CPU.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "");

    Memory mem(11);
    CPU cpu;
    cpu.attachMemory(&mem);

    // Инициализируем R0 = 8, R1 = 2
    cpu.registers[0] = cpu.encodeIntToTritWord(8);
    cpu.registers[1] = cpu.encodeIntToTritWord(3);

    // Устанавливаем значения в память
    mem.set(0, TritWord(Trit::Plus, Trit::Plus, Trit::Zero)); // INC R0
    mem.set(1, TritWord(Trit::Plus, Trit::Plus, Trit::Zero)); // INC R0
    mem.set(2, TritWord(Trit::Minus, Trit::Minus, Trit::Zero)); // DEC R0
    mem.set(3, TritWord(Trit::Plus, Trit::Zero, Trit::Plus)); // LOAD
    mem.set(4, cpu.encodeIntToTritWord(7));                  // адрес
    mem.set(5, TritWord(Trit::Minus, Trit::Zero, Trit::Plus)); // JMP
    mem.set(6, cpu.encodeIntToTritWord(8));                  // адрес
    mem.set(7, cpu.encodeIntToTritWord(10));                 // значение

    // ADD/SUB:
    mem.set(8, TritWord(Trit::Zero, Trit::Plus, Trit::Plus)); // ADD R0 + R1
    mem.set(9, TritWord(Trit::Zero, Trit::Minus, Trit::Minus)); // SUB R0 - R1
    mem.set(10, TritWord(Trit::Plus, Trit::Zero, Trit::Zero)); // HALT

    cpu.run();

    std::cout << "R0 в конце: " << cpu.registers[0].toString() << std::endl;
}

