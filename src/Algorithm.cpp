#include "Algorithm.hpp"

namespace Krypton::Detail {
ByteArray PKCS1EncodeImpl(const ByteArray& plain, size_t k)
{
    auto len = plain.length();
    auto offset = k - len;
    ByteArray res(k, static_cast<byte>(0));
    auto ptr = res.data();
    memcpy(ptr + offset, plain.data(), len);
    res[0] = 0x00;
    res[1] = 0x02;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution dist(1, 255);
    for (size_t i = 2; i < offset - 1; ++i) {
        res[i] = dist(rng);
    }
    return res;
}
ByteArray PKCS1DecodeImpl(const ByteArray& cipher)
{
    if (cipher[0] != 0x00 || cipher[1] != 0x02)
        throw PKCS1Exception();
    size_t i = 2;
    while (i < cipher.length() && cipher[i] != 0x00)
        ++i;
    if (i == cipher.length())
        throw PKCS1Exception();
    ++i;
    return { cipher.data() + i, cipher.length() - i };
}
}