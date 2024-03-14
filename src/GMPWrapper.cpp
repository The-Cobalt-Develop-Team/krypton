// The GNU Multi Precision Wrapper implementation of Krypton Project.
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
#include "GMPWrapper.hpp"

#include "Utilities.hpp"
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
BigInt::BigInt(BigInt&& other) noexcept { mpz_swap(data_, other.data_); }
BigInt::BigInt(const ByteArray& other)
{
    mpz_init(data_);
    mpz_import(data_, other.size(), 1, 1, 0, 0, reinterpret_cast<const void*>(other.data()));
}
BigInt& BigInt::operator=(const BigInt& rhs)
{
    mpz_set(this->data_, rhs.data_);
    return *this;
}
BigInt& BigInt::operator=(BigInt&& rhs) noexcept
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

BigInt BigInt::power(size_t pow, const BigInt& mod) const
{
    BigInt res;
    mpz_powm_ui(res.data_, this->data_, pow, mod.data_);
    return res;
}

BigInt BigInt::power(const BigInt& pow, const BigInt& mod) const
{
    BigInt res;
    mpz_powm(res.data_, this->data_, pow.data_, mod.data_);
    return res;
}

ByteArray BigInt::toByteArray(size_t offset) const
{
    size_t len = (mpz_sizeinbase(this->data_, 2) + 7) / 8;
    ByteArray res(len + offset, 0);
    mpz_export(res.data() + offset, NULL, 1, 1, 0, 0, this->data_);
    return res;
}

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

std::string BigInt::toString() const
{
    return this->toString(10);
}
std::string BigInt::toString(size_t base) const
{
    std::string res(mpz_sizeinbase(this->data_, base), 0);
    mpz_get_str(res.data(), base, this->data_);
    return res;
}

void swap(BigInt& lhs, BigInt& rhs) { mpz_swap(lhs.data_, rhs.data_); }
std::string to_string(const BigInt& bi)
{
    char* ptr = mpz_get_str(NULL, 10, bi.data_);
    return std::string(ptr);
}
std::string to_string(const BigInt& bi, unsigned int base)
{
    char* ptr = mpz_get_str(NULL, base, bi.data_);
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
