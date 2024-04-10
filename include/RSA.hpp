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

#include "Algorithm.hpp"
#include "CPSBase.hpp"
#include "GMPWrapper.hpp"
#include "Hash.hpp"
#include "Utilities.hpp"

namespace Krypton {

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

// using RSAKeyPair = Detail::RSAImpl::RSAKeyPair;
// using RSAKeyPair = std::pair<const ByteArray&, const ByteArray&>;

using RSAKeyPair = std::pair<ByteArray, ByteArray>;

template <typename KeyPair>
struct GetRSAKeySize {
    template <typename... Args>
    size_t operator()(Args&&... args)
    {
        RSAKeyPair p = KeyPair {}(std::forward<Args>(args)...);
        return p.first.size();
    }
};

template <typename N, typename D>
struct BindRSAKeyPair {
    template <typename... Args>
    RSAKeyPair operator()(Args&&... args)
    {
        return { N {}(std::forward<Args>(args)...), D {}(std::forward<Args>(args)...) };
    }
};

template <typename Plain, typename KeyPair>
struct RawRSAEncrypt {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        Detail::RSAImpl ctx;
        RSAKeyPair key = KeyPair {}(std::forward<Args>(args)...);
        ctx.setPrivateKey(key.first, key.second);
        return ctx.encrypt(Plain {}(std::forward<Args>(args)...));
    }
};

template <typename Prev, typename KeyPair>
struct RawRSADecrypt {
    template <typename... Args>
    ByteArray operator()(Args&&... args)
    {
        Detail::RSAImpl ctx;
        auto key = KeyPair {}(std::forward<Args>(args)...);
        ctx.setPublicKey(key.first, key.second);
        return ctx.decrypt(Prev {}(std::forward<Args>(args)...));
    }
};

template <typename Plain, typename KeyPair>
using OAEPRSAEncrypt = RawRSAEncrypt<OAEPEncode<Plain, GetRSAKeySize<KeyPair>>, KeyPair>;
template <typename Plain, typename KeyPair>
using OAEPRSADecrypt = OAEPDecode<RawRSADecrypt<Plain, KeyPair>>;
template <typename Plain, typename KeyPair>
using PKCS1RSAEncrypt = RawRSAEncrypt<PKCS1Encode<Plain, GetRSAKeySize<KeyPair>>, KeyPair>;
template <typename Plain, typename KeyPair>
using PKCS1RSADecrypt = PKCS1Decode<RawRSADecrypt<Plain, KeyPair>>;

}