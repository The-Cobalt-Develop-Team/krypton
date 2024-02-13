#include "BigInteger.hpp"
#include "MathCommon.hpp"

#include <iostream>

int main()
{
    using Krypton::probablePrime;
    using Krypton::BigInt;
    using namespace std;
    cout << boolalpha << 3 << ' ' << probablePrime<uint64_t>(3) << endl;
    cout << boolalpha << 2147483648 << ' ' << probablePrime<uint64_t>(2147483648) << endl;
    cout << boolalpha << 2 << ' ' << probablePrime<uint64_t>(2) << endl;
    cout << boolalpha << 65537 << ' ' << probablePrime<uint64_t>(65537) << endl;
    // cout << boolalpha << probablePrime(BigInt<128>(2147483647)) << endl;
    return 0;
}
