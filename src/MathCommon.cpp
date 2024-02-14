#include "MathCommon.hpp"

namespace Krypton {

PrimeState& PrimeState::getInstance()
{
    static PrimeState ins_;
    return ins_;
}

PrimeState::PrimeType PrimeState::nthPrime(size_t n) { return PrimeState::getInstance().nthPrimeImpl(n); }

PrimeState::PrimeType PrimeState::nthPrimeImpl(size_t n)
{
    if (n > kMaxN)
        return static_cast<PrimeState::PrimeType>(-1);
    if (prime_[n] != 0)
        return prime_[n];
    for (; cur_p_ < kMaxP && cur_n_ < n; ++cur_p_) {
        if (!vis_[cur_p_])
            prime_[++cur_n_] = cur_p_;
        for (size_t i = 1; i <= cur_n_; ++i) {
            if (prime_[i] * cur_p_ > kMaxP)
                break;
            vis_[cur_p_ * prime_[i]] = true;
            if (cur_p_ % prime_[i] == 0)
                break;
        }
    }
    return prime_[n];
}

}