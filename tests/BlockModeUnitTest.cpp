#include "BlockMode.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using namespace Krypton;

TEST(BlockCipherModeTest, AESECBEncryptTest1)
{
    auto key = "2b7e151628aed2a6abf7158809cf4f3c"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto expciph = "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"_ba;
    Detail::ECBContext<Detail::AES128Context> ctx;
    ctx.setKey(key);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res, expciph);
}

TEST(BlockCipherModeTest, AESECBDecryptTest1)
{
    auto key = "2b7e151628aed2a6abf7158809cf4f3c"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "3ad77bb40d7a3660a89ecaf32466ef97f5d3d58503b9699de785895a96fdbaaf43b1cd7f598ece23881b00e3ed0306887b0c785e27e8ad3f8223207104725dd4"_ba;
    Detail::ECBContext<Detail::AES128Context> ctx;
    ctx.setKey(key);
    auto res = ctx.decrypt(ciph);
    ASSERT_EQ(res, plain);
}

TEST(BlockCipherModeTest, AESCBCEncryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "f58c4c04d6e5f1ba779eabfb5f7bfbd69cfc4e967edb808d679f777bc6702c7d39f23369a9d9bacfa530e26304231461b2eb05e2c39be9fcda6c19078c6a9d1b"_ba;
    Detail::CBCContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res, ciph);
}

TEST(BlockCipherModeTest, AESCBCDecryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "f58c4c04d6e5f1ba779eabfb5f7bfbd69cfc4e967edb808d679f777bc6702c7d39f23369a9d9bacfa530e26304231461b2eb05e2c39be9fcda6c19078c6a9d1b"_ba;
    Detail::CBCContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    auto res = ctx.decrypt(ciph);
    ASSERT_EQ(res, plain);
}
