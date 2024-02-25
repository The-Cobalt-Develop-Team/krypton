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
BigInt RSAImpl::encrypt(BigInt m) const { return power(std::move(m), key_.e, key_.n); }
BigInt RSAImpl::decrypt(BigInt c) const { return power(std::move(c), key_.d, key_.n); }

}
