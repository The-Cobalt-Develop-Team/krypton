#pragma once

#include <array>
#include <bitset>
#include <cstdint>
#include <random>

namespace Krypton {

template <typename BaseType, typename PowType>
inline BaseType power(BaseType base, const PowType& pow)
{
    BaseType res { 1 };
    while (pow > 0) {
        if (pow % 2 == 1)
            res = res * base;
        base = base * base;
        pow /= 2;
    }
    return res;
}

template <typename BaseType, typename PowType>
inline BaseType power(BaseType base, PowType pow, const BaseType& mod)
{
    BaseType res { 1 };
    while (pow > PowType(0)) {
        if (pow % 2 == PowType(1))
            res = res * base % mod;
        base = base * base % mod;
        pow = pow / 2;
    }
    return res;
}

template <typename NumType, typename RNGType>
inline NumType getRandom(RNGType& rng, const NumType& min, const NumType& max)
{
    return NumType::random(rng, min, max);
}

template <typename RNGType>
inline uint64_t getRandom(RNGType& rng, uint64_t min, uint64_t max)
{
    static std::uniform_int_distribution<uint64_t> distr(min, max);
    return distr(rng);
}

template <typename NumType, size_t certainty = 16>
inline bool probablePrime(const NumType& n)
{
    if (n >= NumType(1 << 30)) {
        for (size_t i = 2000; i > 0; --i) {
            if (n % PrimeState::nthPrime(i) == NumType(0))
                return false;
        }
    }
    std::random_device rd;
    std::mt19937_64 rng(rd());
    if (n < NumType(3) || n % 2 == NumType(0))
        return n == NumType(2);
    NumType u = n - NumType(1);
    uint64_t t = 0;
    while (u % 2 == NumType(0)) {
        u = u / 2;
        ++t;
    }
    for (size_t i = 0; i < certainty; ++i) {
        auto base = getRandom(rng, NumType(2), n - 1);
        auto v = power(base, u, n);
        if (v == NumType(1))
            continue;
        uint64_t s;
        for (s = 0; s < t; ++s) {
            if (v == n - 1)
                break;
            v = v * v % n;
        }
        if (s == t)
            return false;
    }
    return true;
}

class PrimeState {
private:
    PrimeState()
        : cur_p_(2)
        , cur_n_(0)
    {
    }
    PrimeState(const PrimeState&) = delete;
    PrimeState(PrimeState&&) = delete;
    PrimeState& operator=(const PrimeState&) = delete;

public:
    using PrimeType = unsigned long long;
    static constexpr const PrimeType kMaxP = 200000;
    static constexpr const size_t kMaxN = 10000;
    static constexpr const PrimeType kErr = static_cast<PrimeType>(-1);
    static PrimeState& getInstance();
    static PrimeType nthPrime(size_t n);
    template <typename NumType>
    static NumType nthPrimorial(size_t n)
    {
        if (n > kMaxN)
            return NumType(kErr);
        NumType res(1);
        for (size_t i = 1; i <= n; ++i)
            res = res * PrimeState::nthPrime(i);
        return res;
    }

private:
    PrimeType nthPrimeImpl(size_t n);
    std::bitset<kMaxP + 1> vis_;
    std::array<PrimeType, kMaxN + 1> prime_;
    size_t cur_p_, cur_n_;
};

template <typename NumType, typename RNGType>
inline NumType getRandomPrime(RNGType& rng, const NumType& min, const NumType& max)
{
    // TODO: P!(i) +- 2^k
    NumType randbi;
    do {
        randbi = getRandom(rng, min, max);
        if (randbi % 2 == NumType(0))
            randbi = randbi + NumType(1);
    } while (!probablePrime(randbi));
    return randbi;
}

}
