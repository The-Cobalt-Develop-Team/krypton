#include <gtest/gtest.h>

#include "GMPWrapper.hpp"

using namespace Krypton::GMPWrapper;

TEST(GMPWrapperTest, ArthmeticTest1)
{
    auto a = 3_bi;
    auto b = 32767_bi;
    ASSERT_EQ((a + b).to_ui(), 32770);
    ASSERT_EQ((b - a).to_ui(), 32764);
    ASSERT_TRUE(a + b == 32770_bi);
    ASSERT_EQ((a * b).to_ui(), 98301);
    auto c = a - b;
    ASSERT_EQ(c.sign(), -1);
    c = -c;
    ASSERT_EQ(c.sign(), 1);
}

TEST(GMPWrapperTest, ArthmeticTest2)
{
    auto a = "1145141919810997964573290138836491047593"_bi;
    auto b = "19346392857506940277294860747382004678425"_bi;
    auto c = "20491534777317938241868150886218495726018"_bi;
    ASSERT_EQ(a + b, c);
    c = "-18201250937695942312721570608545513630832"_bi;
    ASSERT_EQ(a - b, c);
    c = "18201250937695942312721570608545513630832"_bi;
    ASSERT_EQ(c, b - a);
    c = "22154365458263276374233195893115983113358748753396026306243541898262155335281025"_bi;
    ASSERT_EQ(a * b, c);
}

TEST(GMPWrapperTest, ArthmeticTest3)
{
    auto a = 0x4f92_bi << 1024;
    auto b = 0x1920_bi << 1024;
    auto c = 131019840_bi << 2048;
    ASSERT_EQ(a * b, c);
}

TEST(GMPWrapperTest, DivisionTest1)
{
    auto a = "109904715240033873"_bi;
    auto b = 3_bi;
    auto c = "36634905080011291"_bi;
    auto d = "109937"_bi;
    ASSERT_EQ(a / b, c);
    ASSERT_EQ(a % b, 0_bi);
    ASSERT_EQ(a / 3, c);
    ASSERT_EQ(a % 3, 0_bi);
    ASSERT_EQ(a / d, "999706333991"_bi);
    ASSERT_EQ(a % d, 65306_bi);
}

TEST(GMPWrapperTest, BitTest1)
{
    auto a = "19385720af8ec9d8"_bix;
    auto b = "adcef927d01f2742"_bix;
    auto mask = 0xff00ff00_bi;
    ASSERT_EQ(a & b, "650859346184241472"_bi);
    ASSERT_EQ(a | b, "13690660464510365658"_bi);
    ASSERT_EQ(a.low_n_bit(8), 0xd8_bi);
    ASSERT_EQ(b & (mask << 16), "f900d0000000"_bix);
}

TEST(GMPWrapperTest, BitTest2)
{
    auto a = "10011010110101100101001011001011"_bib;
    auto m = "00100100101101001011100111000110"_bib;
    auto r = "10111110011000101110101100001101"_bib;
    ASSERT_EQ(a ^ m, r);
}

TEST(GMPWrapperTest, RandomTest1)
{
    std::random_device rd;
    std::mt19937_64 rng(rd());
    auto minr = "1923857230"_bi, maxr = "1028347302048"_bi;
    auto randb = BigInt::random(rng, minr, maxr);
    ASSERT_TRUE(minr <= randb && randb <= maxr);
    auto randb2 = BigInt::random(rng, minr, maxr);
    ASSERT_TRUE(randb != randb2);
}
