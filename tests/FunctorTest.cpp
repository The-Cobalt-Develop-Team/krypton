#include "Hash.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

TEST(FunctorTest, BaseFunctorTest1)
{
    using namespace Krypton;
    struct Input {
        ByteArray operator()() { return toBuffer("KamisatoAyaka"); }
    };
    using func = Base64Encode<Input>;
    auto expres = "S2FtaXNhdG9BeWFrYQ==";
    ASSERT_EQ(func {}(), expres);
}

TEST(FunctorTest, BaseFunctorTest2)
{
    using namespace Krypton;
    struct ToBuffer {
        ByteArray operator()(std::string str) { return toBuffer(str); }
    };
    using func = Base64Encode<ToBuffer>;
    std::string plain = "Yoimiya!";
    auto expres = "WW9pbWl5YSE=";
    ASSERT_EQ(func {}(plain), expres);
}