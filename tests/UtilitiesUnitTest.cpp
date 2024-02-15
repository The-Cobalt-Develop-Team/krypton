#include "Utilities.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace Krypton;

TEST(ByteArrayTest, HexTest)
{
    std::string s = "ab01934f8D2e0f";
    ByteArray buf = fromHex(s);
    ASSERT_EQ(static_cast<uint8_t>(buf[0]), 0xab);
    std::string res = toHex(buf);
    std::string ans = "AB01934F8D2E0F";
    ASSERT_EQ(res, ans);
}

TEST(ByteArrayTest, Base64Test1)
{
    std::string base64 = "WW9pbWl5YSE=";
    ByteArray buf = fromBase64(base64);
    ByteArray res = toBuffer("Yoimiya!");
    ASSERT_EQ(buf, res);
}

TEST(ByteArrayTest, Base64Test2)
{
    ByteArray plain = toBuffer("KamisatoAyaka");
    std::string res = "S2FtaXNhdG9BeWFrYQ==";
    ASSERT_EQ(toBase64(plain), res);
}
