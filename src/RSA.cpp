// The RSA Algorithm implementation of Krypton Project.
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
#include "RSA.hpp"

#include "GMPWrapper.hpp"
#include "MathCommon.hpp"
#include <cassert>

#define MOD(x, y) (x) = (((x) % (y) + (y)) % (y))
namespace Krypton::Detail {

void RSAImpl::generateKeyPair()
{
    std::random_device rd;
    std::mt19937_64 rng(rd());
    auto p = getRandomPrime(rng, 1_bi << 512, 1_bi << 1024);
    auto q = getRandomPrime(rng, 1_bi << 512, 1_bi << 1024);
    key_.e = 65537_bi;
    key_.n = p * q;
    auto lam = (p - 1) * (q - 1);
    BigInt k;
    exgcd(lam, key_.e, k, key_.d);
}
const RSAImpl::RSAKeyPair& RSAImpl::getKeyPair() { return this->key_; }
void RSAImpl::setPrivateKey(const BigInt& n, const BigInt& d)
{
    this->key_.n = n;
    this->key_.d = d;
}
void RSAImpl::setPrivateKey(const BigInt& p, const BigInt& q, const BigInt& dp, const BigInt& dq, const BigInt& qinv)
{
    key_.p = p;
    key_.q = q;
    key_.dp = dp;
    key_.dq = dq;
    key_.qinv = qinv;
    // this->key_.n = key_.p * key_.q;
    // BigInt lam = (key_.p - 1) * (key_.q - 1);
    // BigInt k;
    // exgcd(lam, key_.e, k, key_.d);
    // exgcd(key_.p - 1, key_.e, k, key_.dp);
    // exgcd(key_.q - 1, key_.e, k, key_.dq);
    // exgcd(key_.p, key_.q, k, key_.qinv);
    // assert(key_.dp == dp);
    // assert(key_.dq == dq);
    // assert(key_.qinv == qinv);
}
void RSAImpl::generatePublicKey()
{
    this->key_.n = key_.p * key_.q;
    this->key_.e = 65537;
    BigInt lam = (key_.p - 1) * (key_.q - 1);
    BigInt k;
    exgcd(lam, key_.e, k, key_.d);
    exgcd(key_.p - 1, key_.e, k, key_.dp);
    exgcd(key_.q - 1, key_.e, k, key_.dq);
    exgcd(key_.p, key_.q, k, key_.qinv);
    MOD(key_.d, lam);
    MOD(key_.dp, key_.p - 1);
    MOD(key_.dq, key_.q - 1);
    MOD(key_.qinv, key_.p);
}
BigInt RSAImpl::encrypt(const BigInt &m) const { return m.power(key_.e, key_.n); }
BigInt RSAImpl::decrypt(const BigInt &c) const { return c.power(key_.d, key_.n); }

}
