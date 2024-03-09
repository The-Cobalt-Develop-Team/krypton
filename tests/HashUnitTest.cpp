#include "Hash.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using Krypton::ByteArray;
using Krypton::Detail::MD5HashContext;
using Krypton::Detail::SHA1Context;
using Krypton::Detail::SHA256Context;
using Krypton::operator""_ba;

using namespace Krypton;

TEST(HashTest, MD5Test1)
{
    MD5HashContext ctx;
    ByteArray expres = "61036a99a1e37c5d591c8e92b21444eb"_ba;
    auto buf = fromBase64("WW91IGtub3csIHRoZXJlJ3MgYSByZWNpcGUgdG8gbWFraW5nIGEgcHJvcGVyIHNub3dtYW4KUmVhbGx5PwpVaC1odWghCkEgbGl0dGxlIGJpdCBvZiB5b3UsIGEgbGl0dGxlIGJpdCBvZiBtZQpBIHBhcnQgdGhhdCBsb3ZlcyB0byBkcmVhbQpBIHBhcnQgdGhhdCBzd2luZ3MgZnJvbSBhIHRyZWUKQSBsaXR0bGUgbGlrZSBtZSwgYSBsaXR0bGUgbGlrZSB5b3UKQSBwYXJ0IHRoYXQncyBuaWNlCkEgcGFydCB0aGF0J3MgbmF1Z2h0eSB0b28KQSBsb3lhbCBmcmllbmQgd2hvIGlzIHRoZXJlIG5vIG1hdHRlciB3aGF0CkEgYmlnIHJvdW5kIGJlbGx5CkFuZCBhIGJpZyBib3VuY3kgYnV0dCEKSGUnbGwgbG92ZSB3YXJtIGh1Z3MgYW5kIHRoZSBicmlnaHQgc3VubGlnaHQKQW5kIGhlJ2xsIHJlYWxseSBsb3ZlIHRoZSBzdW1tZXIKQnV0IGhlJ2xsIG1lbHQhClllYWgsIFlvdSdyZSByaWdodApPaCEgU28gd2UnbGwgYnVpbGQgaGltIGJhY2sgdG9nZXRoZXIKWWVzLCB0b2dldGhlciwgdGhhdCdzIHRoZSBrZXkKJ0NhdXNlIGhlJ3MgYSBsaXR0bGUgYml0IG9mIHlvdSBhbmQgbWUKT2theSwgdGltZSBmb3IgYmVkIQpXaGF0PwpObyEgVGltZSBmb3IgbW9yZSBtYWdpYywgcGxlYXNlIGFuZCB0aGFuayB5b3UhCkFubmEsIHlvdSBrbm93IEknbSBub3Qgc3VwcG9zZWQgdG8gZXZlbiBiZSBkb2luZyB0aGlzIQpCdXQgeW91ciBtYWdpYyBpcyB0aGUgbW9zdCBiZWF1dGlmdWwsIHdvbmRlcmZ1bCwgcGVyZmVjdGZ1bCB0aGluZyBpbiB0aGUgd2hvbGUgd2lkZSB3b3JsZCEKRG8geW91IHJlYWxseSB0aGluayBzbz8KWWVzISBTbyBkbyBpdCwgcGxlYXNlLCBiZWZvcmUgSSBidXJzdCBmcm9tIGluc2lkZSB0byBvdXRzaWRlIQpPa2F5LCBva2F5LCBkb24ndCBidXJzdCEKV2hvYSEKQSBsaXR0bGUgYml0IG9mIHlvdSwgYSBsaXR0bGUgYml0IG9mIG1lCllvdSBkbyB0aGUgbWFnaWMgYW5kIEkgZ2V0IHRvIHNlZQpBIGxpdHRsZSBiaXQgb2YgZnVuCkEgbGl0dGxlIGJpdCBvZiBmdW4gaW4gdGhlIG1pZGRsZSBvZiB0aGUgbmlnaHQKQSBsaXR0bGUgYml0IG9mIG1hZ2ljIGFuZCBpdCBhbGwgdGFrZXMgZmxpZ2h0CkxhIGxhIGxhIGxhbGEgbGFsYWxhIGxhIGxhClRoaXMgaXMgc28gYW1hemluZyEgTW9yZSBtb3JlIG1vcmUhCkEgbGl0dGxlIGJpdCBvZiB5b3UhCk1hZ2ljISBNYWdpYyEgRG8gaXQsIEVsc2EsIGRvIGl0IG1vcmUhCkEgbGl0dGxlIGJpdCBvZiBtZQpNZSBtZSBtZSBtZSBtZSBtZSBtZQpMaXR0bGUgYml0IG9mIHlvdSwgbGl0dGxlIGJpdCBvZiBtZSEKTG9vayBhdCBtZSEKQW5uYSE=");
    auto res = ctx.hash(buf);
    ASSERT_EQ(res, expres);
}

TEST(HashTest, SHA1Test1)
{
    SHA1Context ctx {};
    ByteArray expres = "4fc67490b5f5b0a5fae0465809e35d2baf7ec247"_ba;
    auto buf = toBuffer("Yoimiya!");
    auto res = ctx.hash(buf);
    ASSERT_EQ(toHex(res), toHex(expres));
}

TEST(HashTest, SHA256Test1)
{
    SHA256Context ctx {};
    ByteArray expres = "0286d38bcfb643142bac5dcdb3c01d251be51cb4803fcdb95d7a45baeaac389c"_ba;
    auto buf = toBuffer("Yoimiya!");
    auto res = ctx.hash(buf);
    ASSERT_EQ(toHex(res), toHex(expres));
}
