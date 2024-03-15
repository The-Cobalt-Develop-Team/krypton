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
void RSAImpl::setPrivateKey(const BigInt& n, const BigInt& d, size_t keylen)
{
    this->key_.n = n;
    this->key_.d = d;
    this->key_.keylen = keylen;
}
void RSAImpl::setPrivateKey(const BigInt& p, const BigInt& q, const BigInt& dp, const BigInt& dq, const BigInt& qinv, size_t keylen)
{
    key_.p = p;
    key_.q = q;
    key_.dp = dp;
    key_.dq = dq;
    key_.qinv = qinv;
    key_.keylen = keylen;
    this->generatePublicKey();
}
void RSAImpl::setPrivateKey(const ByteArray& n, const ByteArray& d)
{
    key_.keylen = n.length() * 8;
    key_.d = BigInt(d);
    key_.n = BigInt(n);
}
void RSAImpl::setPublicKey(const ByteArray& n, const ByteArray& e)
{
    key_.keylen = n.length() * 8;
    key_.e = BigInt(e);
    key_.n = BigInt(n);
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
ByteArray RSAImpl::encrypt(const ByteArray& m) const
{
    auto mbi = BigInt(m);
    auto cbi = encryptImpl(mbi);
    auto offset = (this->key_.keylen / 8) - ((cbi.size_in_base() + 7) / 8);
    return cbi.toByteArray(offset);
}
ByteArray RSAImpl::decrypt(const ByteArray& c) const
{
    auto len = key_.keylen / 8;
    auto plain = decryptImpl(BigInt(c));
    return plain.toByteArray(len - (plain.size_in_base() + 7) / 8);
}
BigInt RSAImpl::encryptImpl(const BigInt& m) const { return m.power(key_.e, key_.n); }
BigInt RSAImpl::decryptImpl(const BigInt& c) const { return c.power(key_.d, key_.n); }

}
