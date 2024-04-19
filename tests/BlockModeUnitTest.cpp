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

TEST(BlockCipherModeTest, AESCFB8EncryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d"_ba;
    auto cipher = "dc1f1a8520a64db55fcc8ac554844e889700"_ba;
    Detail::CFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    ctx.setSegment(1);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res, cipher);
}

TEST(BlockCipherModeTest, AESCFB8DecryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d"_ba;
    auto cipher = "dc1f1a8520a64db55fcc8ac554844e889700"_ba;
    Detail::CFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    ctx.setSegment(1);
    auto res = ctx.decrypt(cipher);
    ASSERT_EQ(res, plain);
}

TEST(BlockCipherModeTest, AESCFB128EncryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto cipher = "dc7e84bfda79164b7ecd8486985d386039ffed143b28b1c832113c6331e5407bdf10132415e54b92a13ed0a8267ae2f975a385741ab9cef82031623d55b1e471"_ba;
    Detail::CFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    ctx.setSegment(16);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res, cipher);
}

TEST(BlockCipherModeTest, AESCFB128DecryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto cipher = "dc7e84bfda79164b7ecd8486985d386039ffed143b28b1c832113c6331e5407bdf10132415e54b92a13ed0a8267ae2f975a385741ab9cef82031623d55b1e471"_ba;
    Detail::CFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    ctx.setSegment(16);
    auto res = ctx.decrypt(cipher);
    ASSERT_EQ(res, plain);
}

TEST(BlockCipherModeTest, AESOFBEncryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "dc7e84bfda79164b7ecd8486985d38604febdc6740d20b3ac88f6ad82a4fb08d71ab47a086e86eedf39d1c5bba97c4080126141d67f37be8538f5a8be740e484"_ba;
    Detail::OFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res, ciph);
}

TEST(BlockCipherModeTest, AESOFBDecryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto iv = "000102030405060708090a0b0c0d0e0f"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "dc7e84bfda79164b7ecd8486985d38604febdc6740d20b3ac88f6ad82a4fb08d71ab47a086e86eedf39d1c5bba97c4080126141d67f37be8538f5a8be740e484"_ba;
    Detail::OFBContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setIV(iv);
    auto res = ctx.decrypt(ciph);
    ASSERT_EQ(res, plain);
}

TEST(BlockCipherModeTest, AESCTREncryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto nonce = "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "601ec313775789a5b7a7f504bbf3d228f443e3ca4d62b59aca84e990cacaf5c52b0930daa23de94ce87017ba2d84988ddfc9c58db67aada613c2dd08457941a6"_ba;
    Detail::CTRContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setNonce(nonce);
    auto res = ctx.encrypt(plain);
    ASSERT_EQ(res.size(),ciph.size());
    ASSERT_EQ(res, ciph);
}

TEST(BlockCipherModeTest, AESCTRDecryptTest1)
{
    auto key = "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4"_ba;
    auto nonce = "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"_ba;
    auto plain = "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710"_ba;
    auto ciph = "601ec313775789a5b7a7f504bbf3d228f443e3ca4d62b59aca84e990cacaf5c52b0930daa23de94ce87017ba2d84988ddfc9c58db67aada613c2dd08457941a6"_ba;
    Detail::CTRContext<Detail::AES256Context> ctx;
    ctx.setKey(key);
    ctx.setNonce(nonce);
    auto res = ctx.decrypt(ciph);
    ASSERT_EQ(res, plain);
}

