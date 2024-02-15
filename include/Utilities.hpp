#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>

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

}