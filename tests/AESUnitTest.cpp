#include "AES.hpp"
#include "Utilities.hpp"
#include <cstdint>
#include <gtest/gtest.h>

using Krypton::Detail::AES128Context;
using Krypton::Detail::AES192Context;
using Krypton::Detail::AES256Context;
using Krypton::Detail::BaseAESContext;

TEST(AESTest, BaseAESTest1)
{
    AES256Context ctx;
    std::array<uint8_t, 32> key = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f
    };
    std::array<uint8_t, 16> plain = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff
    };
    ctx.init(plain, key, AES256Context::Encrypt);
    ctx.encrypt();
    std::array<uint8_t, 16> exp_res = {
        0x8e, 0xa2, 0xb7, 0xca,
        0x51, 0x67, 0x45, 0xbf,
        0xea, 0xfc, 0x49, 0x90,
        0x4b, 0x49, 0x60, 0x89
    };
    ASSERT_EQ(ctx.getCipherArray(), exp_res);
}

TEST(AESTest, BaseAESTest2)
{
    using Krypton::operator""_ba;
    AES256Context ctx;
    ctx.setKey("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"_ba);
    ctx.setPlain("00112233445566778899aabbccddeeff"_ba);
    ctx.encrypt();
    auto res = ctx.getCipher();
    auto expres = "8ea2b7ca516745bfeafc49904b496089"_ba;
    ASSERT_EQ(res, expres);
}

TEST(AESTest, BaseAESTest3)
{
    using Krypton::operator""_ba;
    AES256Context ctx;
    ctx.setKey("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"_ba);
    ctx.setCipher("8ea2b7ca516745bfeafc49904b496089"_ba);
    ctx.decrypt();
    auto res = ctx.getPlain();
    auto expres = "00112233445566778899aabbccddeeff"_ba;
    ASSERT_EQ(res, expres);
}
