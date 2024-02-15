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

template <typename PrevFunctor>
class Base64Encode {
public:
    template <typename... Args>
    std::string operator()(const Args&... rest)
    {
        return toBase64(PrevFunctor(std::forward<Args>(rest)...));
    }
};

template <typename PrevFunctor>
class Base64Decode {
public:
    template <typename... Args>
    ByteArray operator()(const Args&... rest)
    {
        return fromBase64(PrevFunctor(std::forward<Args>(rest)...));
    }
};

}