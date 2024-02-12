#include "BigInteger.hpp"
#include <iostream>

using namespace std;
using namespace Krypton;

BigInt<128> a(79), b(53), z(0); // NOLINT
BigInt<128> c(79), d(53), e(-53), f(2); // NOLINT

int main()
{
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
    return 0;
}