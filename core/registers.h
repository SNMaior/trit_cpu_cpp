// registers.h - регистры
#pragma once
#include "trit.h"
#include "../utils/utils.h"
#include "memory.h"


struct Registers {// общие регистры
    tryte data[18];

    tryte& operator[](const tryte& t) {
        int idx = utils::toInt(t);
        if (idx < -13 || idx > 4) {
            throw std::out_of_range("Register index out of range: " + std::to_string(idx));
		}
        // диапазон от -13 до +4 нужно сместить в 0..26
        return data[idx + 13];
    }

    const tryte& operator[](const tryte& t) const {
        int idx = utils::toInt(t);
        if (idx < -13 || idx > 4) {
            throw std::out_of_range("Register index out of range: " + std::to_string(idx));
        }
        return data[idx + 13];
    }
};

tryte symreg[8][2]; // символьные регистры

trit EX; // регистр переполнения

trit logic; // логический регистр	   

struct pc : public tword { // Счётчик команд
	using tword::tword;

    pc& inc() {
		tword tpm = this->tword::inc();
		HI = tpm.HI;
		LO = tpm.LO;
		return *this;
    }
}; 

struct Wide_Reg : public tword { // Широкий регистр
    tword data[9];

    tword& operator[](const tryte& t) {
        int idx = utils::toInt(t);
        if (idx > 13 || idx < 5) {
            throw std::out_of_range("Register index out of range: " + std::to_string(idx));
        }
        // диапазон от 5 до 13 нужно сместить в 0..26
        return data[idx - 5];
    }

    const tword& operator[](const tryte& t) const {
        int idx = utils::toInt(t);
        if (idx < 13 || idx > 5) {
            throw std::out_of_range("Register index out of range: " + std::to_string(idx));
        }
        return data[idx - 5];
    }

    pc wrPc() const { return pc(HI, LO); }   // интерпретировать как адрес
    void fromPc(const pc& address) {
        HI = address.HI;
        LO = address.LO;
    }
};