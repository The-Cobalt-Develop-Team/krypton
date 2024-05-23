// The GNU Multi Precision Wrapper header of Krypton Project.
// Copyright (C) 2024  Renatus Madrigal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once

#include "Utilities.hpp"

#include "minigmp/mini-gmp.h"
#include <random>

namespace Krypton {
namespace GMPWrapper {
    class BigInt;
    BigInt operator""_bi(unsigned long long n);
    BigInt operator""_bi(const char* str, size_t);
    BigInt operator""_bix(const char* str, size_t);
    BigInt operator""_bib(const char* str, size_t);

    class BigInt {
    public:
        BigInt();
        BigInt(unsigned int);
        BigInt(signed int);
        explicit BigInt(const ByteArray& ba);
        BigInt(const char* ptr, int base);
        BigInt(const std::string& str, int base);
        BigInt(const BigInt& other);
        BigInt(BigInt&& other) noexcept;
        BigInt& operator=(const BigInt& rhs);
        BigInt& operator=(BigInt&& rhs) noexcept;

        ~BigInt();

        void realloc(size_t bits);
        [[nodiscard]] BigInt low_n_bit(unsigned int n) const;
        [[nodiscard]] const int sign() const;
        [[nodiscard]] const unsigned int to_ui() const;
        [[nodiscard]] const size_t size_in_base(unsigned int base = 2) const;
        [[nodiscard]] ByteArray toByteArray(size_t offset = 0) const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::string toString(size_t) const;

        BigInt power(size_t pow, const BigInt& mod) const;
        BigInt power(const BigInt& pow, const BigInt& mod) const;

        template <typename RNG>
        static BigInt random(RNG& rng, const BigInt& min, const BigInt& max)
        {
            // FIXME: Change another random algorithm
            BigInt res;
            auto rg = max - min;
            auto n = rg.size_in_base(2);
            std::uniform_int_distribution<int> distr(0, 1);
            do {
                res = 0_bi;
                for (size_t i = 0; i < n; ++i) {
                    if (distr(rng))
                        mpz_setbit(res.data_, i);
                }
            } while (res > rg);
            return res + min;
        }

        friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator-(const BigInt& lhs, unsigned int rhs);
        friend BigInt operator-(const BigInt& lhs);
        friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator/(const BigInt& lhs, unsigned int rhs);
        friend BigInt operator%(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator%(const BigInt& lhs, unsigned int rhs);
        friend BigInt operator>>(const BigInt& lhs, unsigned int rhs);
        friend BigInt operator<<(const BigInt& lhs, unsigned int rhs);
        // friend bool operator<=>(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_); }
        friend bool operator==(const BigInt& lhs, const BigInt& rhs);
        friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
        friend bool operator<(const BigInt& lhs, const BigInt& rhs);
        friend bool operator>(const BigInt& lhs, const BigInt& rhs);
        friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
        friend bool operator>=(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator&(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator|(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);

        friend void swap(BigInt& lhs, BigInt& rhs);
        friend std::string to_string(const BigInt& bi);
        friend std::string to_string(const BigInt& bi, unsigned int base);
        friend BigInt abs(BigInt bi);
        friend std::ostream& operator<<(std::ostream& os, const BigInt& bi);

    private:
        mpz_t data_;
    };
}

using BigInt = GMPWrapper::BigInt;
using GMPWrapper::operator""_bi;
using GMPWrapper::operator""_bix;
using GMPWrapper::operator""_bib;

}