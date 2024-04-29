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

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"
#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace Krypton {

using byte = uint8_t;
// using ByteArray = std::basic_string<byte>;
// using ByteArrayView = std::basic_string_view<byte>;
// using ByteArrayStream = std::basic_stringstream<byte>;
using ByteArray = std::vector<byte>;

std::string toBase64(const ByteArray&);
ByteArray fromBase64(const std::string&);
std::string toHex(const ByteArray&);
ByteArray fromHex(const std::string&);
inline ByteArray toBuffer(const std::string& str) { return { str.cbegin(), str.cend() }; }

inline ByteArray operator""_ba(const char* ptr, size_t len) { return fromHex(std::string(ptr, len)); }

inline size_t operator""_sz(unsigned long long x) { return static_cast<size_t>(x); }

// Note: lhs.size() must equals to rhs.size()
inline ByteArray baxor(const ByteArray& lhs, const ByteArray& rhs)
{
    ByteArray res;
    res = ranges::views::zip(lhs, rhs) | ranges::views::transform([](std::pair<byte, byte> p) { return p.first ^ p.second; }) | ranges::to<ByteArray>();
    return res;
}

// TODO: refactor ByteArrayStream
class ByteArrayStream {
private:
    ByteArray buf;
    size_t cur = 0;

public:
    ByteArrayStream()
        : buf()
    {
    }
    ByteArrayStream(const ByteArray& ba)
        : buf(ba)
    {
    }
    ByteArrayStream(size_t bufs)
        : buf(bufs, static_cast<byte>(0))
    {
    }
    void reset() { cur = 0; }
    ByteArrayStream& operator<<(byte c)
    {
        buf.push_back(c);
        return *this;
    }
    ByteArrayStream& operator<<(const ByteArray& ba)
    {
        // buf.append(ba);
        buf.insert(buf.end(), ba.begin(), ba.end());
        return *this;
    }
    ByteArrayStream& operator>>(byte& c)
    {
        c = buf[cur++];
        return *this;
    }
    ByteArrayStream& operator>>(ByteArray& other)
    {
        auto len = other.size();
        for (size_t i = 0; i < len && cur < buf.size(); ++i, ++cur)
            other[i] = buf[cur];
        return *this;
    }
    // TODO: Refactor
    ByteArrayStream& get(ByteArray& other, size_t len)
    {
        other.resize(len);
        for (size_t i = 0; i < len && cur < buf.size(); ++i, ++cur)
            other[i] = buf[cur];
        return *this;
    }
    byte getc() { return buf[cur++]; }
    size_t get(const void*& buf, size_t len)
    {
        auto remain = this->buf.size() - this->cur;
        if (remain < len)
            len = remain;
        buf = this->buf.data() + this->cur;
        cur += len;
        return len;
    }
    bool eof() const
    {
        return cur >= this->buf.size();
    }
    size_t offset() const { return cur; }
    ByteArray str() { return this->buf; }
};

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

}