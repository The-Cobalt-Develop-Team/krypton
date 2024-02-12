#pragma once

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
inline BaseType power(BaseType base, const PowType& pow, const BaseType& mod)
{
    BaseType res { 1 };
    while (pow > 0) {
        if (pow % 2 == 1)
            res = res * base % mod;
        base = base * base % mod;
        pow /= 2;
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
    std::random_device rd;
    std::mt19937_64 rng(rd());
    if (n < 3 || n % 2 == 0)
        return n == 2;
    NumType u = n - 1;
    uint64_t t = 0;
    while (u % 2 == 0) {
        u /= 2;
        ++t;
    }
    for (size_t i = 0; i < certainty; ++i) {
        auto base = getRandom(rng, NumType(3), n - 1);
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

}
