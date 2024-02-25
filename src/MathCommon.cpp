// The common math functions implementation of Krypton Project.
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