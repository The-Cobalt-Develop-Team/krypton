#include "GMPWrapper.hpp"

#include "minigmp/mini-gmp.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

namespace Krypton::GMPWrapper {

BigInt::BigInt() { mpz_init(data_); }
BigInt::BigInt(unsigned int x) { mpz_init_set_ui(data_, x); }
BigInt::BigInt(signed int x) { mpz_init_set_si(data_, x); }
BigInt::BigInt(const char* ptr, int base) { mpz_init_set_str(data_, ptr, base); }
BigInt::BigInt(const std::string& str, int base) { mpz_init_set_str(data_, str.c_str(), base); }
BigInt::BigInt(const BigInt& other) { mpz_init_set(data_, other.data_); }
BigInt::BigInt(BigInt&& other) { mpz_swap(data_, other.data_); }
BigInt& BigInt::operator=(const BigInt& rhs)
{
    mpz_set(this->data_, rhs.data_);
    return *this;
}
BigInt& BigInt::operator=(BigInt&& rhs)
{
    mpz_swap(this->data_, rhs.data_);
    return *this;
}
BigInt::~BigInt() { mpz_clear(data_); }

void BigInt::realloc(size_t bits)
{
    mpz_realloc2(data_, bits);
}
BigInt BigInt::low_n_bit(unsigned int n) const
{
    BigInt res;
    mpz_fdiv_r_2exp(res.data_, this->data_, n);
    return res;
}
const int BigInt::sign() const { return mpz_sgn(this->data_); }
const unsigned int BigInt::to_ui() const { return mpz_get_ui(this->data_); }
const size_t BigInt::size_in_base(unsigned int base) const { return mpz_sizeinbase(this->data_, base); }

BigInt operator+(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_add(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator-(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_sub(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator-(const BigInt& lhs, unsigned int rhs)
{
    BigInt res;
    mpz_sub_ui(res.data_, lhs.data_, rhs);
    return res;
}
BigInt operator-(const BigInt& lhs)
{
    BigInt res;
    mpz_neg(res.data_, lhs.data_);
    return res;
}
BigInt operator*(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_mul(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator/(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_tdiv_q(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator/(const BigInt& lhs, unsigned int rhs)
{
    BigInt res;
    mpz_tdiv_q_ui(res.data_, lhs.data_, rhs);
    return res;
}
BigInt operator%(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_tdiv_r(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator%(const BigInt& lhs, unsigned int rhs)
{
    BigInt res;
    mpz_tdiv_r_ui(res.data_, lhs.data_, rhs);
    return res;
}
BigInt operator>>(const BigInt& lhs, unsigned int rhs)
{
    BigInt res;
    mpz_fdiv_q_2exp(res.data_, lhs.data_, rhs);
    return res;
}
BigInt operator<<(const BigInt& lhs, unsigned int rhs)
{
    BigInt res;
    mpz_mul_2exp(res.data_, lhs.data_, rhs);
    return res;
}
//  bool operator<=>(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_); }
bool operator==(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) == 0; }
bool operator!=(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) != 0; }
bool operator<(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) < 0; }
bool operator>(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) > 0; }
bool operator<=(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) <= 0; }
bool operator>=(const BigInt& lhs, const BigInt& rhs) { return mpz_cmp(lhs.data_, rhs.data_) >= 0; }
BigInt operator&(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_and(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator|(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_ior(res.data_, lhs.data_, rhs.data_);
    return res;
}
BigInt operator^(const BigInt& lhs, const BigInt& rhs)
{
    BigInt res;
    mpz_xor(res.data_, lhs.data_, rhs.data_);
    return res;
}

void swap(BigInt& lhs, BigInt& rhs) { mpz_swap(lhs.data_, rhs.data_); }
std::string to_string(const BigInt& bi)
{
    char* ptr = NULL;
    mpz_get_str(ptr, 10, bi.data_);
    return std::string(ptr);
}
std::string to_string(const BigInt& bi, unsigned int base)
{
    char* ptr = NULL;
    mpz_get_str(ptr, base, bi.data_);
    return std::string(ptr);
}
BigInt abs(BigInt bi)
{
    mpz_abs(bi.data_, bi.data_);
    return bi;
}
std::ostream& operator<<(std::ostream& os, const BigInt& bi) { return os << to_string(bi); }

BigInt operator""_bi(unsigned long long n) { return BigInt(static_cast<unsigned int>(n)); }
BigInt operator""_bi(const char* str, size_t) { return BigInt(str, 10); }
BigInt operator""_bix(const char* str, size_t) { return BigInt(str, 16); }
BigInt operator""_bib(const char* str, size_t) { return BigInt(str, 2); }

}
