#include "Utilities.hpp"

#include <sstream>
#include <string>

namespace Krypton {

std::string toBase64(const ByteArray& buf)
{
    static constexpr const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static constexpr const char padding = '=';
    std::stringstream ss;
    size_t idx;
    uint32_t x;
    for (idx = 0; idx + 2 < buf.size(); idx += 3) {
        x = (buf[idx] << 16) | (buf[idx + 1] << 8) | (buf[idx + 2]);
        ss << table[(x >> 18) & 0x3f] << table[(x >> 12) & 0x3f] << table[(x >> 6) & 0x3f] << table[x & 0x3f];
    }
    if (idx == buf.size() - 1) {
        x = (buf[idx] << 4);
        ss << table[(x >> 6)] << table[x & 0x3f] << padding << padding;
    } else if (idx == buf.size() - 2) {
        x = (buf[idx] << 10) | (buf[idx + 1] << 2);
        ss << table[(x >> 12) & 0x3f] << table[(x >> 6) & 0x3f] << table[x & 0x3f] << padding;
    }
    return ss.str();
}

ByteArray fromBase64(const std::string& str)
{
    static constexpr const byte table[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 63,
        0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };
    ByteArray buf;
    size_t idx;
    uint32_t x;
    for (idx = 0; idx + 3 < str.length() && str[idx + 3] != '='; idx += 4) {
        x = (table[str[idx]] << 18) | (table[str[idx + 1]] << 12) | (table[str[idx + 2]] << 6) | table[str[idx + 3]];
        buf.push_back(static_cast<byte>((x >> 16) & 0xff));
        buf.push_back(static_cast<byte>((x >> 8) & 0xff));
        buf.push_back(static_cast<byte>(x & 0xff));
    }
    x = 0;
    while (idx < str.length() && str[idx] != '=') {
        x = (x << 6) | table[str[idx]];
        ++idx;
    }
    if (idx == str.length() - 2) {
        buf.push_back(static_cast<byte>((x >> 4) & 0xff));
    } else if (idx == str.length() - 1) {
        buf.push_back(static_cast<byte>((x >> 10) & 0xff));
        buf.push_back(static_cast<byte>((x >> 2) & 0xff));
    } else {
        buf.push_back(static_cast<byte>((x >> 16) & 0xff));
        buf.push_back(static_cast<byte>((x >> 8) & 0xff));
        buf.push_back(static_cast<byte>(x & 0xff));
    }
    return buf;
}

std::string toHex(const ByteArray& buf)
{
    static constexpr const char table[] = "0123456789ABCDEF";
    std::stringstream ss;
    for (uint8_t c : buf) {
        ss << table[c >> 4] << table[c & 0x0f];
    }
    return ss.str();
}

ByteArray fromHex(const std::string& str)
{
    static constexpr const byte table[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
        0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 10, 11, 12, 13, 14, 15
    };
    ByteArray buf;
    buf.reserve(str.length() / 2);
    for (size_t i = 0; i + 1 < str.length(); i += 2) {
        buf.push_back((table[str[i]] << 4) | table[str[i + 1]]);
    }
    return buf;
}

}