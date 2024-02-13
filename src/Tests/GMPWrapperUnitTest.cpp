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
}