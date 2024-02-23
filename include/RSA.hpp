#pragma once

#include "GMPWrapper.hpp"

namespace Krypton {

// TODO: finish rsa template
template <typename TInput>
class RSA { };

namespace Detail {
    class RSAImpl {
    public:
        struct RSAKeyPair {
            BigInt n, e, d;
        };

        RSAImpl() = default;
        RSAImpl(const RSAImpl&) = delete;
        RSAImpl(RSAImpl&&) = delete;
        RSAImpl& operator=(const RSAImpl&) = delete;

        void generateKeyPair();
        [[nodiscard]] BigInt encrypt(BigInt m) const;
        [[nodiscard]] BigInt decrypt(BigInt c) const;

    private:
        RSAKeyPair key_;
    };
}

}