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

TEST(FunctorTest, FunctorFactoryTest1)
{
    using namespace Krypton;
    struct Input {
        ByteArray operator()() { return toBuffer("KamisatoAyaka"); }
    };
    using func = FunctorFactory::Next<Input>::Next<Base64Encode>::Result;
    auto expres = "S2FtaXNhdG9BeWFrYQ==";
    ASSERT_EQ(func {}(), expres);
}

TEST(FunctorTest, FunctorFactoryTest2)
{
    using namespace Krypton;
    struct ToBuffer {
        ByteArray operator()(std::string str) { return toBuffer(str); }
    };
    using func = FunctorFactory::Next<ToBuffer>::Next<Base64Encode>::Result;
    std::string plain = "Yoimiya!";
    auto expres = "WW9pbWl5YSE=";
    ASSERT_EQ(func {}(plain), expres);
}

TEST(FunctorTest, FunctorFactoryTest3)
{
    using namespace Krypton;
    struct StringWrapper {
        std::string operator()(const std::string& str) { return str; }
    };
    // using func = FunctorFactory::Next<ToBuffer>::Next<Base64Encode>::Result;
    using func = FunctorFactory::Next<StringWrapper>::Next<Base64Decode>::Next<SHA1Hash>::Result;
    ByteArray expres = "4fc67490b5f5b0a5fae0465809e35d2baf7ec247"_ba;
    auto input = "WW9pbWl5YSE=";
    ASSERT_EQ(func {}(input), expres);
}
