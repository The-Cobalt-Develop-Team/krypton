#include "Algorithm.hpp"
#include "GMPWrapper.hpp"
#include "RSA.hpp"
#include "Utilities.hpp"
#include <gtest/gtest.h>

using namespace Krypton;

TEST(RSATest, RSAKeyTest1)
{
    BigInt n = "00CB3B7EF3D466335AC0ACF54688A174AEE27869101C006EC8779E30BB0B243C4425900EFA34F7A913D1B63FD9E2030EB9D7FAF48E20F65681B039AC58EE5A72B397729FCCDEF9D5B05FAD19673E158E6819D4B21CD04205DBFDB15ED0ACEAE6728BC93475190E81FABCEAD4B10E0EA4FF96BA47C17AD14ACDAAFD7056F166C67D"_bix;
    BigInt e = 65537;
    auto d = "00CA5BA049DA7834B42D5B4A1204237752572F7BB54D9C64B278E06B37229DDEE01FE2B3F7105C74DEC61648B78322763C37C0AA57069D217F9A5FA2AB1E11B3583682878E202E794CB3A961D429019C620C5CE29763C385B344786A4D38CFCCA1C578B860675D464D17200C68F701D342E2FFE1F0B427C53BBF22E3A9524EBBC5"_bix;
    auto p = "00EB732AF2F7FFC4810EEFD8A6B5C83233FA7A6F7B73F642328D4AC3D5F1FF9CEFFEB7E0738005E4F9D739671180C9C0B36C159FD31CEF10C4DC24C9CA2A507DA7"_bix;
    auto q = "00DCF8781DD4CD84CE0D03D4BB6BDB5DB017AEE89740D8B8EBDC232E32B03A880CA086316F86F517E97185C62BBBB91793BDF29385608BACA416EC21221F0BC73B"_bix;
    auto dp = "00E28EE16706881037F77D87457369FE14D212FA2F1AB87D9E5322FC2A6440886CCEF7747CEF55BCAD441232413F61E3914291D446BA72B7FA15B33890D602B9C5"_bix;
    auto dq = "24B1B228AA04F2EFD0A3793922610C3EC8C007E9CE1FEF1D63B207E2ECF6DB4ED57D534A44EC006B88A9814E13DF59B5F40E0B4753CFFDFC45BE32865B0776AF"_bix;
    auto qinv = "00AE0AC3D5FB9C0A6BC1E6E791B5AE39FB23591E859922C0718BD0439987DD5678C3F3850824DE56442FB58A3832B77B4196D9643CB0386AB8AF13FFEFC99DD931"_bix;
    Detail::RSAImpl rsactx;
    rsactx.setPrivateKey(p, q, dp, dq, qinv, 1024);
    auto kp = rsactx.getKeyPair();
    ASSERT_EQ(kp.n, n);
    ASSERT_EQ(kp.e, e);
    ASSERT_EQ(kp.d, d);
    ASSERT_EQ(kp.dp, dp);
    ASSERT_EQ(kp.dq, dq);
    ASSERT_EQ(kp.qinv, qinv);
}

TEST(RSATest, RSAOAEPDecryptTest1)
{
    /*
-----BEGIN PRIVATE KEY-----
MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAMwpA9ep5OvVcmdJ
tl3tBIcq/lUfUyUCClyDQWe3iHDavXYpOyMRhpqXaW7nS9I21kdw1pflaqEgmmhp
5x81dFJZ4JLrWA2i9r6g4Gz9f0SJwXwiBxaHGbKqp+yZJGKvSiMoswLE6RI4Gt3k
U5ttKH+kTDhB84raz4dM68JE5KwjAgMBAAECgYAQo2MRx2HFqn6JVmOLYg6VaXaS
v1vKT1FC2tuQtU9kSx9XquKL6Bi9LovEIwt11Uw/RD3o4SSAfyS2T7MrQvL4weEI
u667Txfwgpv2G/GNoSe3dL6Z0OQMCe4z+Q3cJ09Dk4+brknV9AXo2WThenSM3tVa
imtrt3WZR27MG4BsAQJBAOTi1Hmo29loYQZH6MLGHaXQIyPNFw7T9PFJGn2jITxM
MnvPKk2dpOvXoL7I2jdry1ljGo9+djZTeAYmu7T4AiMCQQDkWFovkKmr3/afIGoo
0C3m/mdNZ+/eX0qwx3V+AuY6acpjBPRRpNrTxTvvDqrPWtx9CSn6c6LcHhROxT1J
Ak4BAkEAomSVX+QAZ9X40N4ANQEfU9/Crsn6TBjyNqcpVm6BTCi7wMARQZRRyMrq
T7JtsNiYJEl+NMUC1jSOeb7Neu5jhQJBAK6dcbnV4inxBmMBIq07JR7zhIOI68Ed
FbzBinNmPDCHHaFC9KJKcpxmw0I38R7H2Lw0+cjwDKZOyjDCW+8suAECQQDKm32u
lj/XJuW+tg8bkA3+mNjr8AzBJ32RQRUVPqg5CWHdvRkjidEuiuHnkBAjbMiqLZvG
ZgQv61ERk3mwYwlW
-----END PRIVATE KEY-----
    */
    auto cipher = fromBase64("qOqcMd+ILsozrqczpY6FdZ6Xrv54phQGSu7vTm66MAgSKyrrs8CTa+Y0os8QhZQ+s9nDv3zRKl/KREbP0ax4ZAnu4NNyI/MyILdRzgwZJ52NKcNzmegRWVbOP9i/aRHCZSBqYyj0ysDxRqp+14iEQy30nDtf1BryekzJRHW9yu0=");
    auto n = "CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23"_ba;
    auto e = 65537_bi;
    auto d = "10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01"_ba;
    auto p = "E4E2D479A8DBD968610647E8C2C61DA5D02323CD170ED3F4F1491A7DA3213C4C327BCF2A4D9DA4EBD7A0BEC8DA376BCB59631A8F7E763653780626BBB4F80223"_bix;
    auto q = "E4585A2F90A9ABDFF69F206A28D02DE6FE674D67EFDE5F4AB0C7757E02E63A69CA6304F451A4DAD3C53BEF0EAACF5ADC7D0929FA73A2DC1E144EC53D49024E01"_bix;
    auto dp = "A264955FE40067D5F8D0DE0035011F53DFC2AEC9FA4C18F236A729566E814C28BBC0C011419451C8CAEA4FB26DB0D89824497E34C502D6348E79BECD7AEE6385"_bix;
    auto dq = "AE9D71B9D5E229F106630122AD3B251EF3848388EBC11D15BCC18A73663C30871DA142F4A24A729C66C34237F11EC7D8BC34F9C8F00CA64ECA30C25BEF2CB801"_bix;
    auto qinv = "CA9B7DAE963FD726E5BEB60F1B900DFE98D8EBF00CC1277D914115153EA8390961DDBD192389D12E8AE1E79010236CC8AA2D9BC666042FEB51119379B0630956"_bix;
    Detail::RSAImpl rsactx;
    rsactx.setPrivateKey(n, d);
    ASSERT_EQ(BigInt(d), rsactx.getKeyPair().d);
    auto plain = rsactx.decrypt(cipher);
    auto raw_plain = fromBase64("AI0wmR96ON87hBdNHF/WWE7knpYR5XN+kKsqNJ0Yi5EM/WlFzlP6Xen4TUg6yLb91D24yY4VN7/ekG/potWaeNKfNydcrwuzjuuS0PtuLgZjlJVPdXlDjERT2K4p8hpLDzvN2TZGNaKFk6UzNVUiTclYWSw/H/q+vMujBqOCd7o=");
    ASSERT_EQ(raw_plain, plain);
    auto res = Detail::OAEPDecodeImpl(plain);
    ASSERT_EQ(res, toBuffer("Yoimiya!"));
}

TEST(RSATest, RSAOAEPDecryptTest2)
{
    auto plain = toBuffer("Yoimiya!");
    auto n = "CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23"_ba;
    auto e = "10001"_ba;
    auto d = "10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01"_ba;
    Detail::RSAImpl rsactx;
    rsactx.setPublicKey(n, e);
    rsactx.setPrivateKey(n, d);
    ASSERT_EQ(rsactx.getKeyPair().keylen, 1024);
    auto oaepplain = fromHex("0009D30672496D6180F51746A6A0E0D26C35988E1E632E289CD38DD349B817ABF9D64CD9D1AF4D46E9BC2E41855649A54F8ACDD345A5F46914B9377DAD9B4BB6ACB096AEE1780E87421F033C860CC2FC67E12AD23B4AA20B92B064890D4224191E2294A211C5FDDA2FCC12FF3367642B1398294AA4E98BC684570F656FC62CDA");
    ASSERT_EQ(Detail::OAEPDecodeImpl(oaepplain), plain);
    auto ciph = fromHex("87ebd1561594affda39044477f9c7369f0b11c0a10918c599a2d3d87e0a5cdc25f89ea9914e8cb96d61cefa5a360dcbc42d415d73c1f83c52e2d9e820f9e15f221a1686bac08b79e0dd4f2b8c7f27cd7699c11752a6b481f3c5d7f75553a59045e9aeea8f11f438e50d05ada51cfefc5e580380985faab62ab960e9eb49a5b3f");
    ASSERT_EQ(ciph.size(), rsactx.getKeyLen() / 8);
    auto dersa = rsactx.decrypt(ciph);
    ASSERT_EQ(toHex(dersa), toHex(oaepplain));
    auto dec = Detail::OAEPDecodeImpl(dersa);
    ASSERT_EQ(dec, plain);
}

TEST(RSATest, RSARawEncryptTest1)
{
    auto n = fromHex("CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23");
    auto e = fromHex("10001");
    auto d = fromHex("10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01");
    Detail::RSAImpl rsactx;
    rsactx.setPublicKey(n, e);
    rsactx.setPrivateKey(n, d);
    ASSERT_EQ(rsactx.getKeyPair().keylen, 1024);
    auto oaepplain = fromHex("0009D30672496D6180F51746A6A0E0D26C35988E1E632E289CD38DD349B817ABF9D64CD9D1AF4D46E9BC2E41855649A54F8ACDD345A5F46914B9377DAD9B4BB6ACB096AEE1780E87421F033C860CC2FC67E12AD23B4AA20B92B064890D4224191E2294A211C5FDDA2FCC12FF3367642B1398294AA4E98BC684570F656FC62CDA");
    auto expciph = fromHex("87ebd1561594affda39044477f9c7369f0b11c0a10918c599a2d3d87e0a5cdc25f89ea9914e8cb96d61cefa5a360dcbc42d415d73c1f83c52e2d9e820f9e15f221a1686bac08b79e0dd4f2b8c7f27cd7699c11752a6b481f3c5d7f75553a59045e9aeea8f11f438e50d05ada51cfefc5e580380985faab62ab960e9eb49a5b3f");
    auto rsacipher = rsactx.encrypt(oaepplain);
    ASSERT_EQ(toHex(rsacipher), toHex(expciph));
}

TEST(RSATest, RSAOAEPEncryptTest1)
{
    auto n = fromHex("CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23");
    auto e = fromHex("10001");
    auto d = fromHex("10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01");
    Detail::RSAImpl rsactx;
    rsactx.setPublicKey(n, e);
    rsactx.setPrivateKey(n, d);
    ASSERT_EQ(rsactx.getKeyPair().keylen, 1024);
    auto plain = toBuffer("Yoimiya!");
    auto oaepciph = rsactx.encrypt(Detail::OAEPEncodeImpl(plain, rsactx.getKeyLen() / 8));
    auto dersa = Detail::OAEPDecodeImpl(rsactx.decrypt(oaepciph));
    ASSERT_EQ(dersa, plain);
}

TEST(RSATest, RSAPKCS1EncryptTest1)
{
    auto n = fromHex("CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23");
    auto e = fromHex("10001");
    auto d = fromHex("10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01");
    Detail::RSAImpl rsactx;
    rsactx.setPublicKey(n, e);
    rsactx.setPrivateKey(n, d);
    ASSERT_EQ(rsactx.getKeyPair().keylen, 1024);
    auto plain = toBuffer("Yoimiya!");
    auto pkcs1ciph = rsactx.encrypt(Detail::PKCS1EncodeImpl(plain, rsactx.getKeyLen() / 8));
    auto dersa = Detail::PKCS1DecodeImpl(rsactx.decrypt(pkcs1ciph));
    ASSERT_EQ(dersa, plain);
}

TEST(RSATest, RSAPKCS1EncryptFunctorTest1)
{
    auto n = fromHex("CC2903D7A9E4EBD5726749B65DED04872AFE551F5325020A5C834167B78870DABD76293B2311869A97696EE74BD236D64770D697E56AA1209A6869E71F35745259E092EB580DA2F6BEA0E06CFD7F4489C17C2207168719B2AAA7EC992462AF4A2328B302C4E912381ADDE4539B6D287FA44C3841F38ADACF874CEBC244E4AC23");
    auto e = fromHex("10001");
    auto d = fromHex("10A36311C761C5AA7E8956638B620E95697692BF5BCA4F5142DADB90B54F644B1F57AAE28BE818BD2E8BC4230B75D54C3F443DE8E124807F24B64FB32B42F2F8C1E108BBAEBB4F17F0829BF61BF18DA127B774BE99D0E40C09EE33F90DDC274F43938F9BAE49D5F405E8D964E17A748CDED55A8A6B6BB77599476ECC1B806C01");
    auto plain = toBuffer("Yoimiya!");
    using enc = PKCS1RSAEncrypt<GetKthArgument<0>, GetKthArgument<1>>;
    using dec = PKCS1RSADecrypt<GetKthArgument<0>, GetKthArgument<1>>;
    RSAKeyPair pubk = { n, e };
    RSAKeyPair privk = { n, d };
    auto kl = GetRSAKeySize<GetKthArgument<0>> {}(pubk);
    ASSERT_EQ(kl, 128);
    kl = GetRSAKeySize<GetKthArgument<0>> {}(privk);
    ASSERT_EQ(kl, 128);
    auto pkcs1ciph = enc {}(plain, pubk);
    auto dersa = dec {}(pkcs1ciph, privk);
    ASSERT_EQ(dersa, plain);
}
