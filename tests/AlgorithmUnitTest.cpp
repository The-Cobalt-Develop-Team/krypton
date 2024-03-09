#include "Algorithm.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using Krypton::Detail::MGF1Impl;
using Krypton::operator""_ba;
using Krypton::toBuffer;
using Krypton::toHex;

TEST(AlgorithmTest, MGFTest1)
{
    auto res = MGF1Impl::mgf(toBuffer("foo"), 3);
    auto expres = "1ac907"_ba;
    ASSERT_EQ(toHex(res), toHex(expres));
}

TEST(AlgorithmTest, MGFTest2)
{
    auto res = MGF1Impl::mgf(toBuffer("bar"), 50);
    auto expres = "bc0c655e016bc2931d85a2e675181adcef7f581f76df2739da74faac41627be2f7f415c89e983fd0ce80ced9878641cb4876"_ba;
    ASSERT_EQ(toHex(res), toHex(expres));
}

TEST(AlgorithmTest, OAEPTest1)
{
    auto buf = toBuffer("Yoimiya!");
    auto oaep = Krypton::Detail::OAEPEncodeImpl(buf, 1024);
    ASSERT_EQ(oaep.length(), 1024);
    auto deoaep = Krypton::Detail::OAEPDecode(oaep);
    ASSERT_EQ(deoaep, buf);
}
