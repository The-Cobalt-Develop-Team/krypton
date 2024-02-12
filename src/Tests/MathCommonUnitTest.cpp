#include <gtest/gtest.h>

#include "MathCommon.hpp"

TEST(PrimeTest, MillerRabinTest1){
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(3));
    ASSERT_FALSE(Krypton::probablePrime<uint64_t>(2147483648));
    ASSERT_FALSE(Krypton::probablePrime<uint64_t>(84797));
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(2));
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(65537));
}