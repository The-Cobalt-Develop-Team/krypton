#pragma once

#include "minigmp/mini-gmp.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

namespace Krypton {
namespace GMPWrapper {
    class BigInt {
    public:
        BigInt() { mpz_init(data_); }
        explicit BigInt(unsigned int x) { mpz_init_set_ui(data_, x); }
        explicit BigInt(signed int x) { mpz_init_set_si(data_, x); }
        BigInt(const char* ptr, int base) { mpz_init_set_str(data_, ptr, base); }
        BigInt(const std::string& str, int base) { mpz_init_set_str(data_, str.c_str(), base); }
        BigInt(const BigInt& other) { mpz_init_set(data_, other.data_); }
        BigInt(BigInt&& other) { mpz_swap(data_, other.data_); }

        ~BigInt() { mpz_clear(data_); }

        void realloc(size_t bits)
        {
            mpz_realloc2(data_, bits);
        }
        BigInt low_n_bit(unsigned int n) const
        {
            BigInt res;
            mpz_fdiv_r_2exp(res.data_, this->data_, n);
            return res;
        }
        const int sign() const { return mpz_sgn(this->data_); }
        const unsigned int to_ui() const { return mpz_get_ui(this->data_); }

        template <typename RNG>
        static BigInt&& random(RNG&, const BigInt& lhs, const BigInt& rhs)
        {
            // TODO: random algorithm
        }

        friend BigInt operator+(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_add(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator-(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_sub(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator-(const BigInt& lhs)
        {
            BigInt res;
            mpz_neg(res.data_, lhs.data_);
            return res;
        }
        friend BigInt operator*(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_mul(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator/(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_tdiv_q(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator/(const BigInt& lhs, unsigned int rhs)
        {
            BigInt res;
            mpz_tdiv_q_ui(res.data_, lhs.data_, rhs);
            return res;
        }
        friend BigInt operator%(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_tdiv_r(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator%(const BigInt& lhs, unsigned int rhs)
        {
            BigInt res;
            mpz_tdiv_r_ui(res.data_, lhs.data_, rhs);
            return res;
        }
        friend BigInt operator>>(const BigInt& lhs, unsigned int rhs)
        {
            BigInt res;
            mpz_fdiv_q_2exp(res.data_, lhs.data_, rhs);
            return res;
        }
        friend BigInt operator<<(const BigInt& lhs, unsigned int rhs)
        {
            BigInt res;
            mpz_mul_2exp(res.data_, lhs.data_, rhs);
            return res;
        }
        // friend bool operator<=>(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_); }
        friend bool operator==(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) == 0; }
        friend bool operator<(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) < 0; }
        friend bool operator>(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) > 0; }
        friend BigInt operator&(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_and(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator|(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_ior(res.data_, lhs.data_, rhs.data_);
            return res;
        }
        friend BigInt operator^(const BigInt& lhs, const BigInt& rhs)
        {
            BigInt res;
            mpz_xor(res.data_, lhs.data_, rhs.data_);
            return res;
        }

        friend void swap(BigInt& lhs, BigInt& rhs) { mpz_swap(lhs.data_, rhs.data_); }
        friend std::string to_string(const BigInt& bi)
        {
            char* ptr = NULL;
            mpz_get_str(ptr, 10, bi.data_);
            return std::string(ptr);
        }
        friend std::string to_string(const BigInt& bi, unsigned int base)
        {
            char* ptr = NULL;
            mpz_get_str(ptr, base, bi.data_);
            return std::string(ptr);
        }
        friend BigInt abs(BigInt bi)
        {
            mpz_abs(bi.data_, bi.data_);
            return std::move(bi);
        }
        friend std::ostream& operator<<(std::ostream& os, const BigInt& bi) { return os << to_string(bi); }

    private:
        mpz_t data_;
    };
    BigInt operator""_bi(unsigned long long n) { return BigInt(static_cast<unsigned int>(n)); }
    BigInt operator""_bi(const char* str, size_t) { return BigInt(str, 10); }
}
}