#include "BigInteger.hpp"
#include <iostream>
#include <random>

using namespace std;
using namespace Krypton;

BigInt<128> a(79), b(53), z(0); // NOLINT
BigInt<128> c(79), d(53), e(-53), f(2); // NOLINT

int main()
{
    std::random_device rd;
    std::mt19937_64 rng(rd());
    a.show();
    b.show();
    z.show();
    (a + b).show();
    (a + b + z).show();
    c.show();
    d.show();
    e.show();
    (c + d).show();
    (c + e).show();
    cout << (c + d).toll() << endl;
    cout << (c + e).toll() << endl;
    (c + e).show();
    (b + z).show();
    // auto randb = BigIntBases::Base2BigInt<128>::random<std::mt19937_64>(rng, BigIntBases::Base2BigInt<128>(0), BigIntBases::Base2BigInt<128>(2147483647));
    auto randb = BigInt<128>::random(rng, BigInt<128>(0), BigInt<128>(2147483647));
    randb.show();
    return 0;
}