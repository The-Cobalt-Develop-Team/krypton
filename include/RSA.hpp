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