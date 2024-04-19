#include "Algorithm.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using Krypton::Detail::MGF1Impl;
using namespace Krypton;

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
    ASSERT_EQ(oaep.size(), 1024);
    auto deoaep = Krypton::Detail::OAEPDecodeImpl(oaep);
    ASSERT_EQ(deoaep, buf);
}

TEST(AlgorithmTest, OAEPTest2)
{
    auto buf = fromBase64("ADKPsdPZzpIkWkwYnJHBttkmMbRTmF/i9b+oNYaTOcUccYzxxMir3rFagyFelOfirG0aqbKZGo4K00rDUI8sgXDdSTL4RE4OBk0BVKjEh3cG5pbwbSqYsysTkgxSlwbHQF6c1FW4qxb9xTFU4RonfQsKG89Z6dwN9EPBglNGZsiZBMwr9oaqyUFEikoA1wleVsoIbIpOpusw62giocAKgjZZm/zCxtguHExKCLIimjlaDQuZ2Cf8AbF+Df3TeiHBPieUIWBESNMqp3AXwYY+RpusGOulOPDjsAXhqw7PSI8/k3mtsFKU8RAp6DutkX4DbeEfOErp2HGg0j3wb9vm7G4n9dXLE9oMHdbUQIMF0u/slTiQXtBZBrOcA8VsVIz57H+wrLmokVJjV59bNNaS1Kzc110qUhJxpHjwZbd7unf8myDbonlmRl/hnqmhuFboALErZeex+M18mxVB4oTqFX6FGrNzLIAUPJKtnvO70eIz6Sod1Oj3n1fJl6moZxzQP/SpDS66ZxxvFDMUZo5KOP6eoyh+bmzHaDSFWtYwSyYLjvvW9mn7aYA9jTN55vXZGQMIoAI2gbGxGTscHRtVTf0PYdM1NHg9GqMhjtCEw7Kt+fFhHIgoos3X+PUISd2QcVttcJ8XI2PsWS9Wstb0iqZdlcuF5O8GglB8IU6HFfMmUF1si4ARLD2v6qQj7gkIdvKOstWEm6y/KsyGhatyR/1NpwU2M4kmykN8436hKWbvMd6Lu2sSCk2RixPZc88347vKUT0UYzEJUjJJEyhZoidAD9i6gbV6JKfSOJo5dqgXBLQYad4/w1GAH5h6uX8UklmKAXKEm2KT7YFJt2ECnPgmEyHp/3AXFEeYMNWJuj9gPolNW1VkI5AWTTFyXy+iHLGqlGfGU2aRiAA25QWMFpWv/aK8KKsttKv2BEv6jKsgYxKTRGJLRDAlK5UBS6WSD/pHwS0/bE6LmWwzLJ8oqWc4bJEMc8CBQIzowxoJ0DIfS0GqUWnzTAtRg2UGHEUlJlBX4foxP6mmbVdrW6TaNnoQ2yb5GXv9Z6+BFmv/0BDiLeNm2HV7IepprKESmAzGUGsqUrnYNeCm24E1fQmGfBavwFtJnzcswNIv4/S/+11amV1wy0+cvFmh58NCGBaaDB3W/o5n30Db2E/ZloO/2eNnubWGjOFdOASAtmBUK40U0uEoA7l+ynUCWhbfkefmGJ5ud9s7z0otXE3g4UzhirOdlmpfzEiL4x8kHtVOU7lTU0hZOatcNeuybxSQcXrQfFIqrg+XOAU76WNNE03oVM6O+Ksxa7ys3Z1dzw4bTRpTJVEuSoHPlOHAmp6stpUqYMFJTaJDwYu/230EiwMgOQ==");
    ASSERT_EQ(buf.size(), 1024);
    auto deoaep = Krypton::Detail::OAEPDecodeImpl(buf);
    auto res = toBuffer("Yoimiya!");
    ASSERT_EQ(deoaep, res);
}
