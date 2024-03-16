// The RSA Algorithm header of Krypton Project.
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

#include "GMPWrapper.hpp"
#include "Hash.hpp"
#include "Utilities.hpp"

namespace Krypton {

// TODO: finish rsa template
template <typename TInput>
class RSA { };

namespace Detail {
    class RSAImpl {
    public:
        struct RSAKeyPair {
            BigInt n, e, d, p, q, dp, dq, qinv;
            // keylen is the size of n in bits
            size_t keylen;
        };

        RSAImpl() = default;
        RSAImpl(const RSAImpl&) = delete;
        RSAImpl(RSAImpl&&) = delete;
        RSAImpl& operator=(const RSAImpl&) = delete;

        void generateKeyPair();
        const RSAKeyPair& getKeyPair();
        size_t getKeyLen() const { return this->key_.keylen; }
        void setPrivateKey(const BigInt& n, const BigInt& d, size_t keylen);
        void setPrivateKey(const BigInt& p, const BigInt& q, const BigInt& dp, const BigInt& dq, const BigInt& qinv, size_t keylen);
        void setPrivateKey(const ByteArray& n, const ByteArray& d);
        void setPublicKey(const ByteArray& n, const ByteArray& e);
        [[nodiscard]] ByteArray decrypt(const ByteArray& c) const;
        [[nodiscard]] ByteArray encrypt(const ByteArray& m) const;

    private:
        void generatePublicKey();
        [[nodiscard]] BigInt encryptImpl(const BigInt& m) const;
        [[nodiscard]] BigInt decryptImpl(const BigInt& c) const;
        RSAKeyPair key_;
    };
}

}