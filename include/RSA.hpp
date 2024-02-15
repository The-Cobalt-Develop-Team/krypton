#pragma once

#include "GMPWrapper.hpp"
#include "MathCommon.hpp"

namespace Krypton {

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
        BigInt encrypt(BigInt m);
        BigInt decrypt(BigInt c);

    private:
        RSAKeyPair key_;
    };
}

}