#include "MathCommon.hpp"

#include <iostream>

// bool millerRabin(int n) {
//   if (n < 3 || n % 2 == 0) return n == 2;
//   int u = n - 1, t = 0;
//   while (u % 2 == 0) u /= 2, ++t;
//   for (int i = 0; i < test_time; ++i) {
//     int a = rand() % (n - 2) + 2, v = quickPow(a, u, n);
//     if (v == 1) continue;
//     int s;
//     for (s = 0; s < t; ++s) {
//       if (v == n - 1) break;  // 得到平凡平方根 n-1，通过此轮测试
//       v = (long long)v * v % n;
//     }
//     if (s == t) return 0;
//   }
//   return 1;
// }

int main()
{
    using Krypton::probablePrime;
    using namespace std;
    cout << boolalpha << 3 << ' ' << probablePrime<uint64_t>(3) << endl;
    cout << boolalpha << 2147483648 << ' ' << probablePrime<uint64_t>(2147483648) << endl;
    cout << boolalpha << 2 << ' ' << probablePrime<uint64_t>(2) << endl;
    cout << boolalpha << 65537 << ' ' << probablePrime<uint64_t>(65537) << endl;
    return 0;
}
