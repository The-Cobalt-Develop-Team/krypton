// The Hash algorithms header of Krypton Project.
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

#include "Utilities.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace Krypton {

namespace Detail {

    class MD5HashContext {
    private:
        struct MD5ContextImpl {
            uint32_t count[2];
            uint32_t state[4]; // 加密结果
            uint8_t buffer[64];
        };
        static constexpr const uint32_t t[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x2441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };

        static constexpr const uint32_t s[4][4] {
            { 7, 12, 17, 22 },
            { 5, 9, 14, 20 },
            { 4, 11, 16, 23 },
            { 6, 10, 15, 21 }
        };

        static constexpr uint8_t kPadding[] = {
            0x80, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        };

        void init();
        void update(const uint8_t* input, uint32_t inputlen);
        void final();

        static void transform(uint32_t state[4], const uint8_t block[64]);
        static void encode(uint8_t* output, const uint32_t* input, uint32_t len);
        static void decode(uint32_t* output, const uint8_t* input, uint32_t len);

        MD5ContextImpl* context;
        uint8_t digest[16];

    public:
        static constexpr const size_t digestLen = 16;
        MD5HashContext()
            : context(new MD5ContextImpl)
            , digest()
        {
        }
        ~MD5HashContext() { delete context; }
        ByteArray hash(const ByteArray& inp);
    };

    class SHA1Context {
    private:
        static constexpr const uint32_t k[] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };
        uint32_t h[5];
        uint8_t digest[20];
        uint8_t* buffer;
        size_t bufsize;

        static uint32_t f(size_t t, uint32_t b, uint32_t c, uint32_t d);
        void init();
        void initBuffer(const ByteArray& inp);
        void updateBlock(const uint8_t* block);
        void update(const uint8_t* buf, size_t len);

    public:
        // len of digest in bytes
        static constexpr const size_t digestLen = 20;
        SHA1Context() = default;
        ~SHA1Context() { delete[] buffer; }
        ByteArray hash(const ByteArray& inp);
    };

    class SHA256Context {
    private:
        static constexpr const uint32_t k[] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
            0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
            0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
            0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
            0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
            0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
        uint32_t h[8];
        uint8_t digest[32];
        uint8_t* buffer;
        size_t bufsize;

        void init();
        void initBuffer(const ByteArray& inp);
        void updateBlock(const uint8_t* block);
        void update(const uint8_t* buf, size_t len);

    public:
        static constexpr const size_t digestLen = 32;
        SHA256Context() = default;
        ~SHA256Context() { delete[] buffer; }
        ByteArray hash(const ByteArray& inp);
    };

}

template <typename PrevFunctor>
struct MD5Hash {
    template <typename... Args>
    ByteArray operator()(Args&&... rest)
    {
        Detail::MD5HashContext ctx {};
        return ctx.hash(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

template <typename PrevFunctor>
struct SHA1Hash {
    template <typename... Args>
    ByteArray operator()(Args&&... rest)
    {
        Detail::SHA1Context ctx {};
        return ctx.hash(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

template <typename PrevFunctor>
struct SHA256Hash {
    template <typename... Args>
    ByteArray operator()(Args&&... rest)
    {
        Detail::SHA256Context ctx {};
        return ctx.hash(PrevFunctor {}(std::forward<Args>(rest)...));
    }
};

}