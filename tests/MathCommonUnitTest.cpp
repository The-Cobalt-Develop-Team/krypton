#include <gtest/gtest.h>

#include "GMPWrapper.hpp"
#include "MathCommon.hpp"

TEST(PrimeTest, MillerRabinTest1)
{
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(3));
    ASSERT_FALSE(Krypton::probablePrime<uint64_t>(2147483648));
    ASSERT_FALSE(Krypton::probablePrime<uint64_t>(84797));
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(2));
    ASSERT_TRUE(Krypton::probablePrime<uint64_t>(65537));
}

TEST(PrimeTest, NthPrimeTest1)
{
    using namespace Krypton;
    ASSERT_EQ(PrimeState::nthPrime(1), 2);
    ASSERT_EQ(PrimeState::nthPrime(3), 5);
    ASSERT_EQ(PrimeState::nthPrime(7), 17);
    ASSERT_EQ(PrimeState::nthPrime(100000), PrimeState::kErr);
}

TEST(PrimeTest, RandomPrimeTest1)
{
    using Krypton::GMPWrapper::operator""_bi;
    std::random_device rd;
    std::mt19937_64 rng(rd());
    auto randp = Krypton::getRandomPrime(rng, 100_bi, 1000_bi);
    ASSERT_TRUE(Krypton::probablePrime(randp));
    ASSERT_TRUE(100_bi <= randp && randp <= 1000_bi);
}

TEST(PrimeTest, RandomPrimeTest2)
{
    using Krypton::GMPWrapper::operator""_bi;
    std::random_device rd;
    std::mt19937_64 rng(rd());
    auto minp = 1_bi << 1023, maxp = (1_bi << 1024);
    auto randp = Krypton::getRandomPrime(rng, minp, maxp);
    ASSERT_TRUE(Krypton::probablePrime(randp));
    ASSERT_TRUE(minp <= randp && randp <= maxp);
}
