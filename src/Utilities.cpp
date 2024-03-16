// The Utilities implementation of Krypton Project.
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
#include "Utilities.hpp"

#include <sstream>
#include <string>

namespace Krypton {

std::string toBase64(const ByteArray& buf)
{
    static constexpr const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    auto bytes_to_encode = buf.data();
    auto in_len = buf.length();
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3]; // store 3 byte of bytes_to_encode
    unsigned char char_array_4[4]; // store encoded character to 4 bytes

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++); // get three bytes (24 bits)
        if (i == 3) {
            // eg. we have 3 bytes as ( 0100 1101, 0110 0001, 0110 1110) --> (010011, 010110, 000101, 101110)
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; // get first 6 bits of first byte,
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4); // get last 2 bits of first byte and first 4 bit of second byte
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6); // get last 4 bits of second byte and first 2 bits of third byte
            char_array_4[3] = char_array_3[2] & 0x3f; // get last 6 bits of third byte

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
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
    size_t i = 0;
    if (str.length() % 2 == 1) {
        buf.push_back(table[str[i]]);
        ++i;
    }
    for (; i + 1 < str.length(); i += 2) {
        buf.push_back((table[str[i]] << 4) | table[str[i + 1]]);
    }
    return buf;
}

}