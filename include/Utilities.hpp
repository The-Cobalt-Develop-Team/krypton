// The Utilities header of Krypton Project.
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

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace Krypton {

using byte = char;
using ByteArray = std::basic_string<byte>;
using ByteArrayView = std::basic_string_view<byte>;
using ByteArrayStream = std::basic_stringstream<byte>;

std::string toBase64(const ByteArray&);
ByteArray fromBase64(const std::string&);
std::string toHex(const ByteArray&);
ByteArray fromHex(const std::string&);
inline ByteArray toBuffer(const std::string& str) { return static_cast<ByteArray>(str); }

inline ByteArray operator""_ba(const char* ptr, size_t len) { return fromHex(std::string(ptr, len)); }

template <typename PrevFunctor>
class Base64Encode {
public:
    template <typename... Args>
    std::string operator()(Args&&... rest)
    {
        return toBase64(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

template <typename PrevFunctor>
class Base64Decode {
public:
    template <typename... Args>
    ByteArray operator()(Args&&... rest)
    {
        return fromBase64(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

struct FunctorFactory {
    template <typename T>
    struct Proxy {
        template <template <typename> typename N>
        using Next = Proxy<N<T>>;
        using Result = T;
    };
    template <typename T>
    using Next = Proxy<T>;
};

}