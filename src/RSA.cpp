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
BigInt RSAImpl::encrypt(BigInt m) { return power(m, key_.e, key_.n); }
BigInt RSAImpl::decrypt(BigInt c) { return power(c, key_.d, key_.n); }

}
