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

TEST(CommonTest, GCDTest1)
{
    using Krypton::GMPWrapper::operator""_bi;
    using Krypton::gcd;
    using Krypton::lcm;
    // P!(300)
    auto a = "36371592218204061058712331794210679383295738036084435324882062175490576728713428158264127600890034416635353241064357553479653251759424062835787755966873734267556309820049566170637114377528203754475483266408424476224668132572485035913234471799160336861668195937035253234653025889850856707292969042980737459368802760719097429739131974095419217225850880068758269263071580115185007606468366081652978874093946489351980513394747029508749147181395993569786523339461407826914143238358455776314166518824613970663711510073543726786906667549535133395027237274225698723475430423486994677648553976762460792612232159167284896048602198605948921687190618560848124663557780365447326869768665325089796729033076125565602369842272356960858976374947619622083916822369676050152667961671931879367523129535315883888958127063782014457131859078721598530290730"_bi;
    // P!(100) * P!(100) * 3
    auto b = "66606875589313619947843706788432463995187901549183059350724725827433389468297538825594274450117732496227820114857399472040785437471605586007231162576314129053621752963230344026073709996496718743066824104425773648934462287818531233654044674563627473726216025497173958690676951244344784102968436468664884276047844035682455955677114584334334796173626587543187316120697084310529092184301064447928188962386764290766252230080696940611677439764300"_bi;
    auto res = "4711930799906184953162487834760260422020574773409675520188634839616415335845034221205289256705544681972439104097777157991804380284218315038719444943990492579030720635990538452312528339864352999310398481791730017201031090"_bi;
    auto lc = "514141276843751500080364612630397130027386132340274916083015356599950648837673385143638251933955425509200479058395191251960809299391285343002841908116695983511683859535603250705835907535793526771962391780805915450314643377222550815819182608858929478064433382803804546111342462106907802877730207913873081480315785661632600734422679077266193215072641557846915722271611240256784011292303975421851288868708402864602654671750969697316843187763841010254471317119381239227707814145772097831002353951114119711355434965105751437525570576850863302199308171816805213087114720926973903026100693625076476211591212193520379057985711829943961538228493754913673686431060003402407120577679639590742983659978552018125344302046691685308478952581911051297667121391620268779753894955779814278850522186971068980097111743220156553142042966501901011640284476905146288505822616684729514129657270592320483836144839425251666930833501768845908815894224219224766717379948823631686444775530314236045933178432031193511806709554797971017124368698046614332529582071550310787460406387100"_bi;
    ASSERT_EQ(gcd(6, 8), 2);
    ASSERT_EQ(lcm(6, 8), 24);
    ASSERT_EQ(gcd(a, b), res);
    ASSERT_EQ(lcm(a, b), lc);
}