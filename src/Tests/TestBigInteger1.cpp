#include "BigInteger.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace BigIntBases;

Base10BigInt a(79), b(53), z(0); // NOLINT
Base2BigInt<128> c(79), d(53), e(-53), f(2); // NOLINT

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
    (c + e).toBase10().show();
    (c - d).toBase10().show();
    e.toBase10().show();
    (c << 1).toBase10().show();
    d.show();
    (d * f).toBase10().show();
    return 0;
}