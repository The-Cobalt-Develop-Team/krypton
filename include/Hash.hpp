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
        SHA1Context() = default;
        ~SHA1Context() { delete[] buffer; }
        ByteArray hash(const ByteArray& inp);
    };

}

template <typename Prev>
struct MD5Hash {
    template <typename... Args>
    ByteArray operator()(const Args&... rest)
    {
        Detail::MD5HashContext ctx {};
        return ctx.hash(PrevFunctor(std::forward<Args>(rest)...));
    }
};

template <typename Prev>
struct SHA1Hash {
    template <typename... Args>
    ByteArray operator()(const Args&... rest)
    {
        Detail::SHA1Context ctx {};
        return ctx.hash(PrevFunctor(std::forward<Args>(rest)...));
    }
};
}