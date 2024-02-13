#pragma once

#include "minigmp/mini-gmp.h"
#include <random>

namespace Krypton {
namespace GMPWrapper {
    class BigInt {
    public:
        BigInt();
        explicit BigInt(unsigned int);
        explicit BigInt(signed int);
        BigInt(const char* ptr, int base);
        BigInt(const std::string& str, int base);
        BigInt(const BigInt& other);
        BigInt(BigInt&& other);
        BigInt& operator=(const BigInt& rhs);
        BigInt& operator=(BigInt&& rhs);

        ~BigInt();

        void realloc(size_t bits);
        BigInt low_n_bit(unsigned int n) const;
        const int sign() const;
        const unsigned int to_ui() const;
        const size_t size_in_base(unsigned int base) const;

        template <typename RNG>
        static BigInt random(RNG& rng, const BigInt& min, const BigInt& max)
        {
            BigInt res = 0_bi;
            auto rg = max - min;
            auto n = rg.size_in_base(2);
            std::uniform_int_distribution<int> distr(0, 1);
            for (size_t i = 0; i < n; ++i) {
                if (distr(rng))
                    mpz_setbit(res.data_, i);
            }
            mpz_and(res.data_, res.data_, rg.data_);
            return res + min;
        }

        friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
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
    BigInt operator""_bi(unsigned long long n);
    BigInt operator""_bi(const char* str, size_t);
    BigInt operator""_bix(const char* str, size_t);
    BigInt operator""_bib(const char* str, size_t);
}
}